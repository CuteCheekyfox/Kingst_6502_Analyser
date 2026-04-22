#include "WDC6502SimulationDataGenerator.h"
#include "WDC6502AnalyzerSettings.h"
#include "WDC6502AnalyzerOpcodes.h"

#define HALFCYCLESAMPLES 40
#define SETUPTIMESAMPLES 5

WDC6502SimulationDataGenerator::WDC6502SimulationDataGenerator()
{
}

WDC6502SimulationDataGenerator::~WDC6502SimulationDataGenerator()
{
}

void WDC6502SimulationDataGenerator::Initialize(U32 simulation_sample_rate, WDC6502AnalyzerSettings *settings)
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mPHI2 = m6502SimulationChannels.Add(settings->mPHI2Channel, mSimulationSampleRateHz, BIT_HIGH);
    mRW = m6502SimulationChannels.Add(settings->mRWChannel, mSimulationSampleRateHz, BIT_HIGH);
    mRUN = m6502SimulationChannels.Add(settings->mRUNChannel, mSimulationSampleRateHz, BIT_HIGH);
    mD0 = m6502SimulationChannels.Add(settings->mD0Channel, mSimulationSampleRateHz, BIT_LOW);
    mD1 = m6502SimulationChannels.Add(settings->mD1Channel, mSimulationSampleRateHz, BIT_LOW);
    mD2 = m6502SimulationChannels.Add(settings->mD2Channel, mSimulationSampleRateHz, BIT_LOW);
    mD3 = m6502SimulationChannels.Add(settings->mD3Channel, mSimulationSampleRateHz, BIT_LOW);
    mD4 = m6502SimulationChannels.Add(settings->mD4Channel, mSimulationSampleRateHz, BIT_LOW);
    mD5 = m6502SimulationChannels.Add(settings->mD5Channel, mSimulationSampleRateHz, BIT_LOW);
    mD6 = m6502SimulationChannels.Add(settings->mD6Channel, mSimulationSampleRateHz, BIT_LOW);
    mD7 = m6502SimulationChannels.Add(settings->mD7Channel, mSimulationSampleRateHz, BIT_LOW);
}

U32 WDC6502SimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested, sample_rate, mSimulationSampleRateHz);
    U8 Counter = 0;
    bool OpCodeCycle = true;

    // We need to start with PHI2 = High
    m6502SimulationChannels.AdvanceAll(HALFCYCLESAMPLES);
    mPHI2->TransitionIfNeeded(BIT_HIGH);
        
    while (mPHI2->GetCurrentSampleNumber() < adjusted_largest_sample_requested) 
    {
        U8 No_of_Bytes = opcodes[Counter].No_of_Opcode_bytes;

        // Move to PHI2 = low
        m6502SimulationChannels.AdvanceAll(HALFCYCLESAMPLES);
        mPHI2->Transition();
        m6502SimulationChannels.AdvanceAll(SETUPTIMESAMPLES);
        CreateDataByte(Counter,false);
        if ( OpCodeCycle == true ) {
            mRUN->TransitionIfNeeded(BIT_HIGH);
            OpCodeCycle = true;
        }
        else {
            mRUN->TransitionIfNeeded(mPHI2->GetCurrentBitState());
        }
        // Move to PHI2 = High 
        m6502SimulationChannels.AdvanceAll(HALFCYCLESAMPLES);
        mPHI2->Transition();
        mRUN->TransitionIfNeeded(mPHI2->GetCurrentBitState());
        
        No_of_Bytes = No_of_Bytes - 1;
        while( No_of_Bytes != 0 ) 
        {
            // Send the data bytes
            U8 Data = No_of_Bytes * 17;
            // Move to PHI2 = low
            m6502SimulationChannels.AdvanceAll(HALFCYCLESAMPLES);
            mPHI2->Transition();
            mRUN->TransitionIfNeeded(mPHI2->GetCurrentBitState());
            m6502SimulationChannels.AdvanceAll(SETUPTIMESAMPLES);
            CreateDataByte(Data,false);
            // Move to PHI2 = High 
            m6502SimulationChannels.AdvanceAll(HALFCYCLESAMPLES);
            mPHI2->Transition();
            mRUN->TransitionIfNeeded(mPHI2->GetCurrentBitState());
            No_of_Bytes = No_of_Bytes - 1;
        }

        // Move to next opcode
        Counter++;
    }

    *simulation_channels = m6502SimulationChannels.GetArray();

    return m6502SimulationChannels.GetCount();
}

void WDC6502SimulationDataGenerator::CreateDataByte(U8 value, bool Write)
{
    // Sets up the data bus and R/W signal
    // Write = true  Set R/W Low
    //       = false Set R/W High
    if ( Write == true ) {
        mRW->TransitionIfNeeded(BIT_LOW);
    }
    else {
        mRW->TransitionIfNeeded(BIT_HIGH);
    }

    // Now setup data byte
    if ( (value>>7 & 0x01) == 1 ) {
        mD7->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD7->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>6 & 0x01) == 1 ) {
        mD6->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD6->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>5 & 0x01) == 1 ) {
        mD5->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD5->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>4 & 0x01) == 1 ) {
        mD4->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD4->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>3 & 0x01) == 1 ) {
        mD3->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD3->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>2 & 0x01) == 1 ) {
        mD2->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD2->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value>>1 & 0x01) == 1 ) {
        mD1->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD1->TransitionIfNeeded(BIT_LOW);
    }
    if ( (value & 0x01) == 1 ) {
        mD0->TransitionIfNeeded(BIT_HIGH);
    } 
    else
    {
        mD0->TransitionIfNeeded(BIT_LOW);
    }
}