#ifndef WDC6502_ANALYZER_SETTINGS
#define WDC6502_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

namespace CycleAnalyzerEnums
{
    enum Mode { Opcode_Read, Data_Read, Data_Write, Not_Valid };
    enum DecodeCycle { Opcode_search, Valid_1_opcode };
    enum CPUTYPES { WDC65C134, WDC65C256 };
};

class WDC6502AnalyzerSettings : public AnalyzerSettings

{
public:
    WDC6502AnalyzerSettings();
    virtual ~WDC6502AnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();

    Channel mPHI2Channel;
    Channel mRWChannel;
    Channel mRUNChannel;
    Channel mD0Channel;
    Channel mD1Channel;
    Channel mD2Channel;
    Channel mD3Channel;
    Channel mD4Channel;
    Channel mD5Channel;
    Channel mD6Channel;
    Channel mD7Channel;
    CycleAnalyzerEnums::CPUTYPES mCPUType;

protected:
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mPHI2ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mRWChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mRUNChannelInterface;

    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD0ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD1ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD2ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD3ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD4ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD5ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD6ChannelInterface;
    std::unique_ptr< AnalyzerSettingInterfaceChannel > mD7ChannelInterface;

    std::unique_ptr< AnalyzerSettingInterfaceNumberList > mCPUTypeInterface;

//    std::auto_ptr< AnalyzerSettingInterfaceChannel > mPHI2ChannelInterface;
//    std::auto_ptr< AnalyzerSettingInterfaceChannel > mRWChannelInterface;
//    std::auto_ptr< AnalyzerSettingInterfaceChannel > mRUNChannelInterface;

//    std::auto_ptr< AnalyzerSettingInterfaceChannel > mInputChannelInterface;
};

#endif //WDC6502_ANALYZER_SETTINGS
