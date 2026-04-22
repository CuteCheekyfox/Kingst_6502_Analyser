#ifndef WDC6502_SIMULATION_DATA_GENERATOR
#define WDC6502_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class WDC6502AnalyzerSettings;

class WDC6502SimulationDataGenerator
{
public:
    WDC6502SimulationDataGenerator();
    ~WDC6502SimulationDataGenerator();

    void Initialize(U32 simulation_sample_rate, WDC6502AnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);

protected:
    WDC6502AnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;

protected: // 6502 specific
    void CreateDataByte(U8 value, bool Write);

    SimulationChannelDescriptorGroup m6502SimulationChannels;
    SimulationChannelDescriptor *mPHI2;
    SimulationChannelDescriptor *mRW;
    SimulationChannelDescriptor *mRUN;
    SimulationChannelDescriptor *mD0;
    SimulationChannelDescriptor *mD1;
    SimulationChannelDescriptor *mD2;
    SimulationChannelDescriptor *mD3;
    SimulationChannelDescriptor *mD4;
    SimulationChannelDescriptor *mD5;
    SimulationChannelDescriptor *mD6;
    SimulationChannelDescriptor *mD7;
};

struct Opcode {
    bool OpcodeCycle;
    U8 OpcodeValue;
};

#endif //WDC6502_SIMULATION_DATA_GENERATOR
