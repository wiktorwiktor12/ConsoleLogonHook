#pragma once

#include <dwrite.h>

namespace DirectUI {

  struct UILIB_API RichText : public Element {
  
    static PropertyInfo * WINAPI BaselineProp(void);
    static IClassInfo * WINAPI GetClassInfoPtr(void);
    static PropertyInfo * WINAPI LineSpacingProp(void);
    static PropertyInfo * WINAPI OverhangOffsetProp(void);
    virtual HRESULT SetMaxLineCount(UINT);
    static PropertyInfo * WINAPI TypographyProp(void);
    static PropertyInfo * WINAPI AliasedRenderingProp(void);
    static PropertyInfo * WINAPI ColorFontPaletteIndexProp(void);
    static PropertyInfo * WINAPI ConstrainLayoutProp(void);
    static PropertyInfo * WINAPI DisableAccTextExtendProp(void);
    static PropertyInfo * WINAPI FontColorRunsProp(void);
    static PropertyInfo * WINAPI FontSizeRunsProp(void);
    static PropertyInfo * WINAPI FontWeightRunsProp(void);
    virtual HRESULT GetColorFontPaletteIndex(void);
    virtual HRESULT GetConstrainLayout(void);
    static PropertyInfo * WINAPI LocaleProp(void);
    static PropertyInfo * WINAPI MapRunsToClustersProp(void);
    virtual void OnEvent(DirectUI::Event *);
    virtual void Paint(HDC__ *,tagRECT const *,tagRECT const *,tagRECT *,tagRECT *);
    static PropertyInfo * WINAPI TypographyRunsProp(void);
    static PropertyInfo * WINAPI VerticalScriptProp(void);
    HRESULT _EnsureLeadDrawOffsetIsSet(void);
    HRESULT _FlushDWrite(void);
    HRESULT _PrepareTextRender(void);
    HRESULT _SetAcceleratorAccentIfAppropriate(void);
    HRESULT _SetTypographyInternal(void);
    HRESULT SetConstrainLayout(int);
    HRESULT GetShortcutChar(void);
    HRESULT SetAliasedRendering(bool);
    HRESULT SetBaseline(int);
    HRESULT SetColorFontPaletteIndex(int);
    HRESULT SetDisableAccTextExtend(bool);
    HRESULT SetFontColorRuns(USHORT const *);
    HRESULT SetFontSizeRuns(USHORT const *);
    HRESULT SetFontWeightRuns(USHORT const *);
    HRESULT SetLineSpacing(int);
    HRESULT SetLocale(USHORT const *);
    HRESULT SetMapRunsToClusters(bool);
    HRESULT SetOverhangOffset(int);
    HRESULT SetTypography(USHORT const *);
    HRESULT SetTypographyRuns(USHORT const *);
    HRESULT SetVerticalScript(bool);
    RichText(void);
    virtual HRESULT CreateCache(UINT,struct IDUIRichTextCache * *);
    virtual SIZE GetContentSize(int,int,DirectUI::Surface *);
    virtual HRESULT GetFactory(void);
    virtual HRESULT GetFontColorRuns(DirectUI::Value * *);
    virtual HRESULT GetForegroundColorRef(ULONG *);
    virtual HRESULT GetLineCount(void);
    virtual HRESULT GetTrimmedLineCount(void);
    virtual HRESULT GetVerticalScript(void);
    virtual HRESULT Initialize(DirectUI::Element *,ULONG *);
    virtual void OnHosted(DirectUI::Element *);
    virtual void OnPropertyChanged(DirectUI::PropertyInfo const *,int,DirectUI::Value *,DirectUI::Value *);
    virtual HRESULT SetCache(ULONG, struct IDUIRichTextCache *);
    virtual HRESULT SetDWriteFontCollection(IDWriteFontCollection *);
    virtual HRESULT SetDWriteTextLayout(IDWriteTextLayout *);
    virtual HRESULT StopUsingCache(void);
    virtual HRESULT _AdjustRangeForPathJoinCharacters(DWRITE_TEXT_RANGE *);
    virtual HRESULT _ApplyDrawTextFlags(void);
    virtual HRESULT _ApplyIntratextFormatting(void);
    virtual HRESULT _BuildRenderStringForDWrite(USHORT const *,USHORT,bool);
    virtual HRESULT _CreateDWriteLayout(tagRECT const *,DirectUI::Value *);
    virtual HRESULT _EnsureTextFormat(void);
    virtual HRESULT _GetDWFontSize(float,bool);
    virtual HRESULT _GetDWFontWeight(void);
    virtual HRESULT _GetDWMetrics(DWRITE_TEXT_METRICS *);
    virtual HRESULT _GetDWOverhangMetrics(DWRITE_OVERHANG_METRICS *);
    virtual HRESULT _GetDefaultOverhang(void);
    virtual HRESULT _GetMinSizeWidth(void);
    virtual HRESULT _GetScaledOverhangOffset(void);
    virtual HRESULT _InitDWrite(void);
    virtual HRESULT _PaintStringContentDWrite(HDC__ *,tagRECT const *,tagRECT const *,DirectUI::Value *);
    virtual HRESULT _SetFontColorRun(USHORT const *,DWRITE_TEXT_RANGE);
    virtual HRESULT _SetFontSizeRun(USHORT const *,DWRITE_TEXT_RANGE);
    virtual HRESULT _SetFontSizeRunsInternal(void);
    virtual HRESULT _SetFontWeightRun(USHORT const *,DWRITE_TEXT_RANGE);
    virtual HRESULT _SetFontWeightRunsInternal(void);
    virtual HRESULT _SetLineSpacingInternal(void);
    virtual HRESULT _SetRangedStringRunsWithValue(USHORT const *,enum StringRunStyles);
    virtual HRESULT _SetStrikethrough(void);
    virtual HRESULT _SetTypographyRun(USHORT const *,DWRITE_TEXT_RANGE);
    virtual HRESULT _SetTypographyRunsInternal(void);
    virtual HRESULT _SetUnderline(void);
    virtual HRESULT _UpdateRangeForClusterMetrics(DWRITE_TEXT_RANGE *);

  };

}
