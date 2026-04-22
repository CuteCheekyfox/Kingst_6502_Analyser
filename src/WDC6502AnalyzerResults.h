#ifndef WDC6502_ANALYZER_RESULTS
#define WDC6502_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class WDC6502Analyzer;
class WDC6502AnalyzerSettings;

class WDC6502AnalyzerResults : public AnalyzerResults
{
public:
    WDC6502AnalyzerResults(WDC6502Analyzer *analyzer, WDC6502AnalyzerSettings *settings);
    virtual ~WDC6502AnalyzerResults();

    virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
    virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
    virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
    virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions
    virtual void GenerateOpcodeTxt(char * Text, size_t Text_Size, U32 RawOpcodeData, U8 Flags);
    virtual void AddFirstPartOpcode(char * Text, size_t Text_Size, U8 OpcodeValue);
    virtual void AddLastPartOpcode(char * Text, size_t Text_Size, U8 OpcodeValue);
protected:  //vars
    WDC6502AnalyzerSettings *mSettings;
    WDC6502Analyzer *mAnalyzer;
};

#endif //WDC6502_ANALYZER_RESULTS
