#ifndef WDC6502_ANALYZER_H
#define WDC6502_ANALYZER_H

#include <Analyzer.h>
#include "WDC6502AnalyzerResults.h"
#include "WDC6502SimulationDataGenerator.h"

#define SINGLE_BYTE_FLAG ( 1 << 0 )
#define TWO_BYTE_FLAG ( 1 << 1 )
#define THREE_BYTE_FLAG ( 1 << 2 )
#define FOUR_BYTE_FLAG ( 1 << 3 )
#define DATA_BYTE_FLAG ( 1 << 4 )

class WDC6502AnalyzerSettings;

struct DataBusRead {
    U64 Sample_Number;
    BitState ReadWrite;
    BitState Run;
    U8 Data_Bus;
};

class ANALYZER_EXPORT WDC6502Analyzer : public Analyzer
{
public:
    WDC6502Analyzer();
    virtual ~WDC6502Analyzer();
    virtual void SetupResults();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);
    virtual U32 GetMinimumSampleRateHz();

    virtual const char *GetAnalyzerName() const;
    virtual bool NeedsRerun();

protected:
    virtual void AdvanceToPHI2Sample();
    U8 CalculateDataBusValue();
    
    virtual void ReadOpcode();
    virtual void ReadDataAccess();
    virtual void ReadData(DataBusRead &DataRead);
    virtual bool IsTheNextByteAnOpcodeFetch();
    
// #pragma warning( push )
// #pragma warning( disable : 4251 ) //warning C4251: 'WDC6502Analyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class

protected: //functions
    void ComputeSampleOffsets();

protected: //vars
    std::unique_ptr< WDC6502AnalyzerSettings > mSettings;
    std::unique_ptr< WDC6502AnalyzerResults > mResults;

    WDC6502SimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    AnalyzerChannelData *mPHI2;
    AnalyzerChannelData *mRW;    
    AnalyzerChannelData *mRUN;
    AnalyzerChannelData *mD0;
    AnalyzerChannelData *mD1;
    AnalyzerChannelData *mD2;
    AnalyzerChannelData *mD3;
    AnalyzerChannelData *mD4;
    AnalyzerChannelData *mD5;
    AnalyzerChannelData *mD6;
    AnalyzerChannelData *mD7;


//#pragma warning( pop )
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //WDC6502_ANALYZER_H
