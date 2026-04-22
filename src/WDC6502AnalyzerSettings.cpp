#include "WDC6502AnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

// #pragma warning(disable: 4800) //warning C4800: 'U32' : forcing value to bool 'true' or 'false' (performance warning)
#define CHANNEL_NAME "Data"

WDC6502AnalyzerSettings::WDC6502AnalyzerSettings()
    :   mPHI2Channel(UNDEFINED_CHANNEL),
        mRWChannel(UNDEFINED_CHANNEL),
        mRUNChannel(UNDEFINED_CHANNEL),
        mD0Channel(UNDEFINED_CHANNEL),
        mD1Channel(UNDEFINED_CHANNEL),
        mD2Channel(UNDEFINED_CHANNEL),
        mD3Channel(UNDEFINED_CHANNEL),
        mD4Channel(UNDEFINED_CHANNEL),
        mD5Channel(UNDEFINED_CHANNEL),
        mD6Channel(UNDEFINED_CHANNEL),
        mD7Channel(UNDEFINED_CHANNEL),
        mCPUType(CycleAnalyzerEnums::WDC65C134)
{    
    mPHI2ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mPHI2ChannelInterface->SetTitleAndTooltip("Phi2", "Phase 2 Clock");
    mPHI2ChannelInterface->SetChannel(mPHI2Channel);

    mRWChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mRWChannelInterface->SetTitleAndTooltip("R/W", "Read/Write Signal");
    mRWChannelInterface->SetChannel(mRWChannel);

    mRUNChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mRUNChannelInterface->SetTitleAndTooltip("Run", "Run output");
    mRUNChannelInterface->SetChannel(mRUNChannel);

    mD0ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD0ChannelInterface->SetTitleAndTooltip("D0", "Data bus");
    mD0ChannelInterface->SetChannel(mD0Channel);
    mD1ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD1ChannelInterface->SetTitleAndTooltip("D1", "Data bus");
    mD1ChannelInterface->SetChannel(mD1Channel);
    mD2ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD2ChannelInterface->SetTitleAndTooltip("D2", "Data bus");
    mD2ChannelInterface->SetChannel(mD2Channel);
    mD3ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD3ChannelInterface->SetTitleAndTooltip("D3", "Data bus");
    mD3ChannelInterface->SetChannel(mD3Channel);
    mD4ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD4ChannelInterface->SetTitleAndTooltip("D4", "Data bus");
    mD4ChannelInterface->SetChannel(mD4Channel);
    mD5ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD5ChannelInterface->SetTitleAndTooltip("D5", "Data bus");
    mD5ChannelInterface->SetChannel(mD5Channel);
    mD6ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD6ChannelInterface->SetTitleAndTooltip("D6", "Data bus");
    mD6ChannelInterface->SetChannel(mD6Channel);
    mD7ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mD7ChannelInterface->SetTitleAndTooltip("D7", "Data bus");
    mD7ChannelInterface->SetChannel(mD7Channel);

    mCPUTypeInterface.reset(new AnalyzerSettingInterfaceNumberList());
    mCPUTypeInterface->SetTitleAndTooltip("CPU base type", "Specify the CPU core to use.");
    mCPUTypeInterface->AddNumber(CycleAnalyzerEnums::WDC65C134, "WDC 65C134","Microcontroller 8-bit core");
    mCPUTypeInterface->AddNumber(CycleAnalyzerEnums::WDC65C256, "WDC 65C256","Microcontroller 16-bit core");
    mCPUTypeInterface->SetNumber(mCPUType);

    AddInterface(mPHI2ChannelInterface.get());
    AddInterface(mRWChannelInterface.get());
    AddInterface(mRUNChannelInterface.get());
    AddInterface(mD0ChannelInterface.get());
    AddInterface(mD1ChannelInterface.get());
    AddInterface(mD2ChannelInterface.get());
    AddInterface(mD3ChannelInterface.get());
    AddInterface(mD4ChannelInterface.get());
    AddInterface(mD5ChannelInterface.get());
    AddInterface(mD6ChannelInterface.get());
    AddInterface(mD7ChannelInterface.get());
    AddInterface(mCPUTypeInterface.get());
    
    AddExportOption(0, "Export as text/csv file");
    AddExportExtension(0, "Text file", "txt");
    AddExportExtension(0, "CSV file", "csv");

    ClearChannels();
    AddChannel(mPHI2Channel, "Phi2", false);
    AddChannel(mRWChannel, "R/W", false);
    AddChannel(mRUNChannel, "Run", false);
    AddChannel(mD0Channel, "D0", false);
    AddChannel(mD1Channel, "D1", false);
    AddChannel(mD2Channel, "D2", false);
    AddChannel(mD3Channel, "D3", false);
    AddChannel(mD4Channel, "D4", false);
    AddChannel(mD5Channel, "D5", false);
    AddChannel(mD6Channel, "D6", false);
    AddChannel(mD7Channel, "D7", false);

}

WDC6502AnalyzerSettings::~WDC6502AnalyzerSettings()
{
}

bool WDC6502AnalyzerSettings::SetSettingsFromInterfaces()
{
    mPHI2Channel    =   mPHI2ChannelInterface->GetChannel();
    mRWChannel      =   mRWChannelInterface->GetChannel();
    mRUNChannel     =   mRUNChannelInterface->GetChannel();
    mD0Channel      =   mD0ChannelInterface->GetChannel();
    mD1Channel      =   mD1ChannelInterface->GetChannel();
    mD2Channel      =   mD2ChannelInterface->GetChannel();
    mD3Channel      =   mD3ChannelInterface->GetChannel();
    mD4Channel      =   mD4ChannelInterface->GetChannel();
    mD5Channel      =   mD5ChannelInterface->GetChannel();
    mD6Channel      =   mD6ChannelInterface->GetChannel();
    mD7Channel      =   mD7ChannelInterface->GetChannel();

    mCPUType        =   CycleAnalyzerEnums::CPUTYPES(U32(mCPUTypeInterface->GetNumber()));

    ClearChannels();
    AddChannel(mPHI2Channel, "Phi2", true);
    AddChannel(mRWChannel, "R/W", true);
    AddChannel(mRUNChannel, "Run", true);
    AddChannel(mD0Channel, "D0", true);
    AddChannel(mD1Channel, "D1", true);
    AddChannel(mD2Channel, "D2", true);
    AddChannel(mD3Channel, "D3", true);
    AddChannel(mD4Channel, "D4", true);
    AddChannel(mD5Channel, "D5", true);
    AddChannel(mD6Channel, "D6", true);
    AddChannel(mD7Channel, "D7", true);

    return true;
}

void WDC6502AnalyzerSettings::UpdateInterfacesFromSettings()
{
    mPHI2ChannelInterface->SetChannel(mPHI2Channel);
    mRWChannelInterface->SetChannel(mRWChannel);
    mRUNChannelInterface->SetChannel(mRUNChannel);
    mD0ChannelInterface->SetChannel(mD0Channel);
    mD1ChannelInterface->SetChannel(mD1Channel);
    mD2ChannelInterface->SetChannel(mD2Channel);
    mD3ChannelInterface->SetChannel(mD3Channel);
    mD4ChannelInterface->SetChannel(mD4Channel);
    mD5ChannelInterface->SetChannel(mD5Channel);
    mD6ChannelInterface->SetChannel(mD6Channel);
    mD7ChannelInterface->SetChannel(mD7Channel);
    mCPUTypeInterface->SetNumber(mCPUType);
}

void WDC6502AnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    const char *name_string;    //the first thing in the archive is the name of the protocol analyzer that the data belongs to.
    text_archive >> &name_string;
    if (strcmp(name_string, "WDC6502Analyzer") != 0) {
        AnalyzerHelpers::Assert("WDC6502Analyzer: Provided with a settings string that doesn't belong to us;");
    }

    text_archive >> mPHI2Channel;
    text_archive >> mRWChannel;
    text_archive >> mRUNChannel;
    text_archive >> mD0Channel;
    text_archive >> mD1Channel;
    text_archive >> mD2Channel;
    text_archive >> mD3Channel;
    text_archive >> mD4Channel;
    text_archive >> mD5Channel;
    text_archive >> mD6Channel;
    text_archive >> mD7Channel;
    U32 Trial = 0;
    text_archive >> Trial;
    mCPUType = (CycleAnalyzerEnums::CPUTYPES)Trial;
    //text_archive >> (U32 &)&mCPUType;

    ClearChannels();
    AddChannel(mPHI2Channel, "Phi2", true);
    AddChannel(mRWChannel, "R/W", true);
    AddChannel(mRUNChannel, "Run", true);
    AddChannel(mD0Channel, "D0", true);
    AddChannel(mD1Channel, "D1", true);
    AddChannel(mD2Channel, "D2", true);
    AddChannel(mD3Channel, "D3", true);
    AddChannel(mD4Channel, "D4", true);
    AddChannel(mD5Channel, "D5", true);
    AddChannel(mD6Channel, "D6", true);
    AddChannel(mD7Channel, "D7", true);

    UpdateInterfacesFromSettings();
}

const char *WDC6502AnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << "WDC6502Analyzer";
    text_archive << mPHI2Channel;
    text_archive << mRWChannel;
    text_archive << mRUNChannel;
    text_archive << mD0Channel;
    text_archive << mD1Channel;
    text_archive << mD2Channel;
    text_archive << mD3Channel;
    text_archive << mD4Channel;
    text_archive << mD5Channel;
    text_archive << mD6Channel;
    text_archive << mD7Channel;
    text_archive << mCPUType;

    return SetReturnString(text_archive.GetString());
}
