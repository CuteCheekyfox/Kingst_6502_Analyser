#include "WDC6502Analyzer.h"
#include "WDC6502AnalyzerSettings.h"
#include "WDC6502AnalyzerOpcodes.h"
#include <AnalyzerChannelData.h>

// Read the data capture and split into data/opcode cycles
//
//  If Opcode then get first byte 
//      If valid get no of bytes in full opcode read
//          Read in other bytes
//      Else
//          Store as data read ( Invalid op-code )
//      Store Frame
//          Type, Data2 and Flags
//  Else ( Data )
//      Get data byte and store as Data read or Write
//      Store Frame
//          Type, Data2    
//
// Data is read/Written on the H to L transition of PHI2
//

WDC6502Analyzer::WDC6502Analyzer()
    : Analyzer(),
        mSettings(new WDC6502AnalyzerSettings()),
        mSimulationInitilized(false),
        mPHI2(NULL),
        mRW(NULL),    
        mRUN(NULL),
        mD0(NULL),
        mD1(NULL),
        mD2(NULL),
        mD3(NULL),
        mD4(NULL),
        mD5(NULL),
        mD6(NULL),
        mD7(NULL)
{
    SetAnalyzerSettings(mSettings.get());
}

WDC6502Analyzer::~WDC6502Analyzer()
{
    KillThread();
}

void WDC6502Analyzer::SetupResults()
{
    //Unlike the worker thread, this function is called from the GUI thread
    //we need to reset the Results object here because it is exposed for direct access by the GUI, and it can't be deleted from the WorkerThread

    mResults.reset(new WDC6502AnalyzerResults(this, mSettings.get()));
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->mPHI2Channel);
}

void WDC6502Analyzer::WorkerThread()
{
    mPHI2 = GetAnalyzerChannelData(mSettings->mPHI2Channel);
    mRW = GetAnalyzerChannelData(mSettings->mRWChannel);
    mRUN = GetAnalyzerChannelData(mSettings->mRUNChannel);
    mD0 = GetAnalyzerChannelData(mSettings->mD0Channel);
    mD1 = GetAnalyzerChannelData(mSettings->mD1Channel);
    mD2 = GetAnalyzerChannelData(mSettings->mD2Channel);
    mD3 = GetAnalyzerChannelData(mSettings->mD3Channel);
    mD4 = GetAnalyzerChannelData(mSettings->mD4Channel);
    mD5 = GetAnalyzerChannelData(mSettings->mD5Channel);
    mD6 = GetAnalyzerChannelData(mSettings->mD6Channel);
    mD7 = GetAnalyzerChannelData(mSettings->mD7Channel);

    // Used at the start of analysis to move to the first
    // detected PHI2 low phase 
    if ( mPHI2->GetBitState() == BIT_LOW ) {
        mPHI2->AdvanceToNextEdge();
    }
    mPHI2->AdvanceToNextEdge();
    AdvanceToPHI2Sample();

    for (; ;) {
        if ( IsTheNextByteAnOpcodeFetch() == true ) {
            ReadOpcode();
            //ReadDataAccess();
        }
        else {
            ReadDataAccess();
        }
        
        CheckIfThreadShouldExit();    
    }    
}

bool WDC6502Analyzer::NeedsRerun()
{
    return false;
}

U32 WDC6502Analyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    if (mSimulationInitilized == false) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 WDC6502Analyzer::GetMinimumSampleRateHz()
{
    return 4;
}

const char *WDC6502Analyzer::GetAnalyzerName() const
{
    return "Kingst 6502 Analyser";
}

const char *GetAnalyzerName()
{
    return "Kingst 6502 Analyser";
}

Analyzer *CreateAnalyzer()
{
    return new WDC6502Analyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
    delete analyzer;
}

void WDC6502Analyzer::AdvanceToPHI2Sample()
{
    // Helper function to advance all the data streams
    // to the same point as the PHI2 clock
    U64 Current_PHI2_Sample = mPHI2->GetSampleNumber();
    mRUN->AdvanceToAbsPosition(Current_PHI2_Sample);
    mRW->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD0->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD1->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD2->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD3->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD4->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD5->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD6->AdvanceToAbsPosition(Current_PHI2_Sample);
    mD7->AdvanceToAbsPosition(Current_PHI2_Sample);
}

U8 WDC6502Analyzer::CalculateDataBusValue()
{
    U8 Byte_Value = 0;
    if (mD7->GetBitState() == BIT_HIGH){
        Byte_Value += 0x80;
    }
    if (mD6->GetBitState() == BIT_HIGH){
        Byte_Value += 0x40;
    }
    if (mD5->GetBitState() == BIT_HIGH){
        Byte_Value += 0x20;
    }
    if (mD4->GetBitState() == BIT_HIGH){
        Byte_Value += 0x10;
    }
    if (mD3->GetBitState() == BIT_HIGH){
        Byte_Value += 0x08;
    }
    if (mD2->GetBitState() == BIT_HIGH){
        Byte_Value += 0x04;
    }
    if (mD1->GetBitState() == BIT_HIGH){
        Byte_Value += 0x02;
    }
    if (mD0->GetBitState() == BIT_HIGH){
        Byte_Value += 0x01;
    }

    return Byte_Value;   
}

void WDC6502Analyzer::ReadOpcode()
{
    U64 frame_starting_sample = 0;
    U64 opcode_debug_sample = 0;
    U64 Current_Edge = 0;
    
    U8 type = CycleAnalyzerEnums::Opcode_Read;
    U8 DataBusValue = 0;
    U64 OpcodeData = 0;
    U8 OpcodeFlags = 0;
    
    DataBusRead FirstByteData;
    DataBusRead SecondByteData;
    DataBusRead ThirdByteData;    
    DataBusRead FourthByteData;    

    // Start with clock in Low state.
    Current_Edge = mPHI2->GetSampleNumber();
    
    frame_starting_sample = Current_Edge;
    
    opcode_debug_sample = frame_starting_sample;

    // Read First byte
    ReadData(FirstByteData);
    DataBusValue = FirstByteData.Data_Bus;
    OpcodeData = (U64)DataBusValue;
    
    // Currently fixed to NMOS 6502 mode 
    CycleAnalyzerEnums::CPUTYPES CPUType = mSettings->mCPUType;
    // tbd allow selection of other cpus of this family.
    // WDC65C256 => All opcodes valid
    // WDC65C134 => 65C02 opcodes
    //
    if (    CPUType == CycleAnalyzerEnums::WDC65C134 &&
            opcodes[OpcodeData].cmos_65C02 == false ) {
        // Opcode is not valid
        type = CycleAnalyzerEnums::Not_Valid;        
    }
    else 
    {
        // Opcode is valid for this device
        U8 No_of_bytes = opcodes[OpcodeData].No_of_Opcode_bytes;
        
        if( No_of_bytes == 1 ) {
            OpcodeFlags = SINGLE_BYTE_FLAG; 
        }
        if( No_of_bytes > 1 ){
            // Get byte two for this opcode
            ReadData(SecondByteData);
            OpcodeData = (OpcodeData << 8) + SecondByteData.Data_Bus; 
            OpcodeFlags = TWO_BYTE_FLAG;      
        }
        if( No_of_bytes > 2 ){
            // Get byte three for this opcode
            ReadData(ThirdByteData);
            OpcodeData = (OpcodeData << 8) + ThirdByteData.Data_Bus; 
            OpcodeFlags = THREE_BYTE_FLAG;      
        }
        if( No_of_bytes > 3 ){
            // Get byte four for this opcode
            ReadData(FourthByteData);
            OpcodeData = (OpcodeData << 8) + FourthByteData.Data_Bus; 
            OpcodeFlags = FOUR_BYTE_FLAG;      
        }
    }
    //ok now record the value!
    //note that we're not using the mData2 or mType fields for anything, so we won't bother to set them.
    Frame frame;
    frame.mStartingSampleInclusive = frame_starting_sample;
    frame.mEndingSampleInclusive = mPHI2->GetSampleNumber();
    frame.mData1 = 0;
    frame.mData2 = OpcodeData;
    frame.mFlags = OpcodeFlags;
    frame.mType = type; 

    mResults->AddMarker(opcode_debug_sample, AnalyzerResults::Dot, mSettings->mRUNChannel);

    mResults->AddFrame(frame);
    mResults->CommitPacketAndStartNewPacket();  
    mResults->CommitResults();  
    ReportProgress(frame.mEndingSampleInclusive);

}

void WDC6502Analyzer::ReadData(DataBusRead &DataRead)
{
    // This expects the PHI2 signal to be low
    // and reads in all the signals.
    // It then moves to the L->H transition and 
    // Finishes by going to the H->L transition.

    mPHI2->AdvanceToNextEdge();
    AdvanceToPHI2Sample(); 
    mPHI2->AdvanceToNextEdge();
    AdvanceToPHI2Sample(); 

    DataRead.Sample_Number = mPHI2->GetSampleNumber();
    DataRead.ReadWrite = mRW->GetBitState();
    DataRead.Run = mRUN->GetBitState();
    DataRead.Data_Bus = CalculateDataBusValue();

}

bool WDC6502Analyzer::IsTheNextByteAnOpcodeFetch()
{
    // This expects the PHI2 signal to be LOW
    // It then looks at the next L->H transition and 
    // checks to see if the RUN signal transitions.
    // Returns False if it does.
    bool Transition_Seen = false;
    U64 NextTransition = mPHI2->GetSampleOfNextEdge();
    
    Transition_Seen = mRUN->WouldAdvancingToAbsPositionCauseTransition(NextTransition);

    return !Transition_Seen; 
}

void WDC6502Analyzer::ReadDataAccess()
{
    U64 data_starting_sample = 0;
    U8 type = CycleAnalyzerEnums::Not_Valid;
    U8 DataBusValue = 0;

    DataBusRead DataByte;
    
    data_starting_sample = mPHI2->GetSampleNumber();
    
    // Read Data byte
    ReadData(DataByte);
    DataBusValue = (U8) DataByte.Data_Bus;
    if ( DataByte.ReadWrite == BIT_LOW ) {
        type = CycleAnalyzerEnums::Data_Write;
    }
    else {
        type = CycleAnalyzerEnums::Data_Read;
    }

    //ok now record the value!
    //note that we're not using the mData2 or mType fields for anything, so we won't bother to set them.
    Frame frame;
    frame.mStartingSampleInclusive = data_starting_sample;
    frame.mEndingSampleInclusive = mPHI2->GetSampleNumber();
    frame.mData1 = 0;
    frame.mData2 = DataBusValue;
    frame.mFlags = 0;
    frame.mType = type; 

    mResults->AddMarker(data_starting_sample, AnalyzerResults::ErrorDot, mSettings->mRUNChannel);

    mResults->AddFrame(frame);
    mResults->CommitPacketAndStartNewPacket();  
    mResults->CommitResults();  
    ReportProgress(frame.mEndingSampleInclusive);
}