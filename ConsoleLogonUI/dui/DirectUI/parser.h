#pragma once

namespace DirectUI
{
	enum _DUI_PARSE_STATE
	{

	};

	typedef _DUI_PARSE_STATE DUI_PARSE_STATE;

	struct LINEINFO
	{

	};

	struct XMLParserCond
	{

	};

	typedef bool(__stdcall* Unknow3Call )(class Element *);

	class UILIB_API Schema
	{
	public:
		enum Pattern
		{
		};

		enum Event
		{

		};

		struct RoleMap
		{

		};

		struct PropertyInfo
		{

		};

		struct PatternMap
		{

		};

		struct EventMap
		{

		};

		struct EventInfo
		{

		};

		struct ControlInfo
		{

		};

		struct PatternInfo
		{

		};

		
		Schema & operator=(Schema const &);
		static long __stdcall CreatePatternProvider(Pattern, ElementProvider *, IUnknown * *);
		static Event __stdcall EventFromEventId(int);
		static long __stdcall Init(void);
		static int __stdcall LookupAccessibleRole(int, bool *);
		static Pattern __stdcall PatternFromPatternId(int);
		static Unknow3Call __stdcall PfnIsSupportedFromPattern(Pattern);

		static int AcceleratorKeyProperty;
		static int AccessKeyProperty;
		static int AsyncContentLoadedEvent;
		static int AutomationFocusChangedEvent;
		static int AutomationIdProperty;
		static int AutomationPropertyChangedEvent;
		static int BoundingRectangleProperty;
		static int ButtonControlType;
		static int CalendarControlType;
		static int CheckBoxControlType;
		static int ClassNameProperty;
		static int ClickablePointProperty;
		static int ComboBoxControlType;
		static int ControlTypeProperty;
		static int CultureProperty;
		static int CustomControlType;
		static int DataGridControlType;
		static int DataItemControlType;
		static int DockPattern;
		static int DocumentControlType;
		static int EditControlType;
		static int ExpandCollapsePattern;
		static int ExpandCollapse_ExpandCollapseState_Property;
		static int FrameworkId;
		static int GridItemPattern;
		static int GridItem_ColumnSpan_Property;
		static int GridItem_Column_Property;
		static int GridItem_Parent_Property;
		static int GridItem_RowSpan_Property;
		static int GridItem_Row_Property;
		static int GridPattern;
		static int Grid_ColumnCount_Property;
		static int Grid_RowCount_Property;
		static int GroupControlType;
		static int HasKeyboardFocusProperty;
		static int HeaderControlType;
		static int HeaderItemControlType;
		static int HelpTextProperty;
		static int HyperlinkControlType;
		static int ImageControlType;
		static int InvokeInvokedEvent;
		static int InvokePattern;
		static int IsContentElementProperty;
		static int IsControlElementProperty;
		static int IsEnabledProperty;
		static int IsKeyboardFocusableProperty;
		static int IsOffscreen;
		static int IsPasswordProperty;
		static int ItemContainerPattern;
		static int ItemStatusProperty;
		static int ItemTypeProperty;
		static int LabeledByProperty;
		static int LayoutInvalidatedEvent;
		static int ListControlType;
		static int ListItemControlType;
		static int LocalizedControlTypeProperty;
		static int MenuBarControlType;
		static int MenuClosedEvent;
		static int MenuControlType;
		static int MenuItemControlType;
		static int MenuOpenedEvent;
		static int MultipleViewPattern;
		static int NameProperty;
		static int NewNativeWindowHandleProperty;
		static int NullControlType;
		static int Orientation;
		static int PaneControlType;
		static int ProcessIdProperty;
		static int ProgressBarControlType;
		static int RadioButtonControlType;
		static int RangeValuePattern;
		static int RangeValue_IsReadOnly_Property;
		static int RangeValue_LargeChange_Property;
		static int RangeValue_Maximum_Property;
		static int RangeValue_Minimum_Property;
		static int RangeValue_SmallChange_Property;
		static int RangeValue_Value_Property;
		static int RuntimeIdProperty;
		static int ScrollBarControlType;
		static int ScrollItemPattern;
		static int ScrollPattern;
		static int Scroll_HorizontalScrollPercent_Property;
		static int Scroll_HorizontalViewSize_Property;
		static int Scroll_HorizontallyScrollable_Property;
		static int Scroll_VerticalScrollPercent_Property;
		static int Scroll_VerticalViewSize_Property;
		static int Scroll_VerticallyScrollable_Property;
		static int SelectionInvalidatedEvent;
		static int SelectionItemElementAddedToSelectionEvent;
		static int SelectionItemElementRemovedFromSelectionEvent;
		static int SelectionItemElementSelectedEvent;
		static int SelectionItemPattern;
		static int SelectionItem_IsSelected_Property;
		static int SelectionItem_SelectionContainer_Property;
		static int SelectionPattern;
		static int Selection_CanSelectMultiple_Property;
		static int Selection_IsSelectionRequired_Property;
		static int Selection_Selection_Property;
		static int SeparatorControlType;
		static int SliderControlType;
		static int SpinnerControlType;
		static int SplitButtonControlType;
		static int StatusBarControlType;
		static int StructureChangedEvent;
		static int TabControlType;
		static int TabItemControlType;
		static int TableControlType;
		static int TableItemPattern;
		static int TableItem_ColumnHeaderItems_Property;
		static int TableItem_RowHeaderItems_Property;
		static int TablePattern;
		static int Table_ColumnHeaders_Property;
		static int Table_RowHeaders_Property;
		static int Table_RowOrColumnMajor_Property;
		static int TextControlType;
		static int TextPattern;
		static int TextTextSelectionChangedEvent;
		static int ThumbControlType;
		static int TitleBarControlType;
		static int TogglePattern;
		static int Toggle_ToggleState_Property;
		static int ToolBarControlType;
		static int ToolTipClosedEvent;
		static int ToolTipControlType;
		static int ToolTipOpenedEvent;
		static int TransformPattern;
		static int TreeControlType;
		static int TreeItemControlType;
		static long(__stdcall* UiaHostProviderFromHwnd)(HWND hwnd, IRawElementProviderSimple ** ppProvider);
		static int(__stdcall* UiaLookupId)(AutomationIdentifierType type, const GUID* pGuid);
		static long(__stdcall* UiaRaiseAutomationEvent)(IRawElementProviderSimple * pProvider, EVENTID id);
		static long(__stdcall* UiaRaiseAutomationPropertyChangedEvent)(IRawElementProviderSimple * pProvider, PROPERTYID id, VARIANT oldValue, VARIANT newValue);
		static long(__stdcall* UiaRaiseStructureChangedEvent)(IRawElementProviderSimple * pProvider, StructureChangeType structureChangeType, int * pRuntimeId, int cRuntimeIdLen);
		static LRESULT(__stdcall* UiaReturnRawElementProvider)(HWND hwnd, WPARAM wParam, LPARAM lParam, IRawElementProviderSimple * el);
		static int ValuePattern;
		static int Value_IsReadOnly_Property;
		static int Value_Value_Property;
		static int VirtualizedItemPattern;
		static int WindowControlType;
		static int WindowPattern;
		static int WindowWindowClosedEvent;
		static int WindowWindowOpenedEvent;
	private:
		static long __stdcall LookupControlInfos(void);
		static long __stdcall LookupEventInfos(void);
		static long __stdcall LookupPatternInfos(void);
		static long __stdcall LookupPropertyInfos(void);
		static long __stdcall GetProcs(void);

		static RoleMap const * __ptr32 const _roleMapping;
		static ControlInfo const * __ptr32 const g_controlInfoTable;
		static EventInfo const * __ptr32 const g_eventInfoTable;
		static EventMap const * __ptr32 const g_eventMapping;
		static bool g_fInited;
		static PatternInfo const * __ptr32 const g_patternInfoTable;
		static PatternMap const * __ptr32 const g_patternMapping;
		static const Schema::PropertyInfo * __ptr32 const g_propertyInfoTable;
	};

	typedef Value* (WINAPI* GetSheetCallback)(UCString, void*);
	typedef void ( WINAPI* ParseErrorCallback)(UCString, UCString, int, void*);
	typedef bool ( WINAPI* UnknownAttrCallback)(UCString, void*);

	namespace ParserTools
	{
		struct ExprNode
		{
		public:

		};

		class ValueParser
		{
		public:
		};
	}

	class UILIB_API DUIXmlParser
	{
	public:
		DUIXmlParser(const DUIXmlParser &);
		DUIXmlParser();
		virtual ~DUIXmlParser();

		DUIXmlParser& operator=( const DUIXmlParser & );
	
		// Callback defines
		/*
		
		*/
		static long WINAPI Create(_Inout_ DUIXmlParser** pXmlParser, _In_opt_ GetSheetCallback, _In_opt_ void*sheetParam, _In_opt_ ParseErrorCallback, _In_opt_ void*parseErrorParam);
		long CreateElement(UCString,class Element*, class Element*, unsigned long*, class Element**out);
		
		void Destroy();
		void EnableDesignMode();
		GetSheetCallback GetGetSheetCallback();
		HINSTANCE GetHInstance();
			
		HINSTANCE GetResourceHInstance();
		long GetSheet(UCString, Value**);
		void* GetSheetContext();
		
		long LookupElement(IXmlReader*, UCString, HINSTANCE, struct IClassInfo**);
		long LookupElement(LINEINFO, UCString, HINSTANCE, struct IClassInfo**);

		void SetDefaultHInstance(HINSTANCE);
		
		void SetGetSheetCallback(GetSheetCallback,void*);
		void SetParseErrorCallback(ParseErrorCallback, void*);
		void SetUnknownAttrCallback(bool (__stdcall*)(UCString, void*), void*);

		long SetPreprocessedXML(UCString, HINSTANCE, HINSTANCE);
		void SetUnavailableIcon(HICON);

		HRESULT SetXML(UCString szXML, HINSTANCE hModule, HINSTANCE/*0x10000000 作用未知*/);

		//其资源必须在"UIFILE"类型中
		HRESULT SetXMLFromResource(UINT uID, HINSTANCE hModule, HINSTANCE/*0x10000000 作用未知*/);
		HRESULT SetXMLFromResource(UCString szID, HINSTANCE hModule, HINSTANCE/*0x10000000 作用未知*/);

		long UpdateSheets(Element*);
		void _DestroyTables();
		long _InitializeTables();

		// exported for: int, unsigned long, Value*, RECT
		template<typename T>
		struct UILIB_API FunctionDefinition
		{
			FunctionDefinition() = delete;
			FunctionDefinition(const FunctionDefinition&) = delete;
			~FunctionDefinition() = delete;

			FunctionDefinition& operator=(const FunctionDefinition &);
		};

		union ParsedArg
		{
			// ??
		};
		
	protected:
		long _EnterOnCurrentThread();
		LINEINFO _GetLineInfo(IXmlReader*);

		long AddRulesToStyleSheet(IXmlReader*, StyleSheet*, UCString, DynamicArray<XMLParserCond, 0>*, DynamicArray<UString, 0>*);
		long CreateLayout(const ParserTools::ExprNode*, long (WINAPI*)(int, int*, Value**));
		long CreateStyleSheet(IXmlReader*, UCString, StyleSheet**);
		long CreateXmlReader(IXmlReader**);
		long CreateXmlReaderFromHGLOBAL(void*, IXmlReader**);
		long CreateXmlReaderInputWithEncodingName(IStream*, UCString, IUnknown**);
		long GetParserCommon(DUIXmlParser**);
		long GetPropValPairInfo(IXmlReader*, IClassInfo*, UCString, UCString, PropertyInfo const **, Value**);
		long GetPropValPairInfo(LINEINFO, IClassInfo*, UCString, UCString, PropertyInfo const **, Value**);
		long GetValueParser(ParserTools::ValueParser**);
		static long WINAPI GetXmlLiteDll(HINSTANCE*);
		long Initialize();
		long InitializeParserFromXmlLiteReader(IXmlReader*);
		static bool WINAPI IsThemeClassName(const ParserTools::ExprNode*);
		long MapPropertyEnumValue(const struct EnumMap*, UCString, int*);
		long MapPropertyNameToPropertyInfo(LINEINFO, IClassInfo*, UCString, const PropertyInfo**);
		long ParseARGBColor(const ParserTools::ExprNode*, unsigned long*);
		long ParseArgs(const ParserTools::ExprNode*, ParsedArg*, unsigned int, const char*);
		long ParseAtomValue(const ParserTools::ExprNode*, Value**);
		long ParseBoolValue(const ParserTools::ExprNode*, Value**);
		long ParseColor(const ParserTools::ExprNode*, unsigned long*);
		long ParseDFCFill(const ParserTools::ExprNode*, Value**);
		long ParseDTBFill(const ParserTools::ExprNode*, Value**);
		long ParseFillValue(const ParserTools::ExprNode*, Value**);
		long ParseFunction(UCString, const ParserTools::ExprNode*, ParsedArg*, unsigned int, const char*);
		long ParseGTCColor(const ParserTools::ExprNode*, unsigned long*);
		long ParseGTFStr(const ParserTools::ExprNode*, Value**);
		long ParseGTMarRect(const ParserTools::ExprNode*, LPRECT);
		long ParseGTMetInt(const ParserTools::ExprNode*, int*);
		long ParseGradientFill(const ParserTools::ExprNode*, Value**);
		long ParseGraphicGraphic(const ParserTools::ExprNode*, Value**);
		long ParseGraphicHelper(bool,const ParserTools::ExprNode*, Value**);
		long ParseGraphicValue(const ParserTools::ExprNode*, Value**);
		long ParseIconGraphic(const ParserTools::ExprNode*, Value**);
		long ParseImageGraphic(const ParserTools::ExprNode*, Value**);
		long ParseIntValue(const ParserTools::ExprNode*, Value**);
		long ParseLayoutValue(const ParserTools::ExprNode*);
		long ParseLibrary(const ParserTools::ExprNode*, HINSTANCE*);
		long ParseLiteral(const ParserTools::ExprNode*, UCString*);
		long ParseLiteralColor(UCString, unsigned long*);
		long ParseLiteralColorInt(UCString, int*);
		long ParseLiteralNumber(UCString, int*);
		long ParseMagnitude(UCString, int*);
		long ParseNumber(const ParserTools::ExprNode*, int*);
		long ParsePointValue(const ParserTools::ExprNode*, Value**);
		long ParseQuotedString(const ParserTools::ExprNode*, UCString*);
		long ParseRGBColor(const ParserTools::ExprNode*, unsigned long*);
		long ParseRect(const ParserTools::ExprNode*, LPRECT);
		long ParseRectRect(const ParserTools::ExprNode*, LPRECT);
		long ParseRectValue(const ParserTools::ExprNode*, Value**);
		long ParseResStr(const ParserTools::ExprNode*, Value**);
		long ParseResid(const ParserTools::ExprNode*, UCString*);
		long ParseSGraphicGraphic(const ParserTools::ExprNode*, Value**);
		long ParseSGraphicHelper(bool,const ParserTools::ExprNode*, Value**);
		long ParseSizeValue(const ParserTools::ExprNode*, Value**);
		long ParseStringValue(const ParserTools::ExprNode*, Value**);
		long ParseStyleSheets(IXmlReader*);
		long ParseSysMetricInt(const ParserTools::ExprNode*, int*);
		long ParseSysMetricStr(const ParserTools::ExprNode*, Value**);
		long ParseTheme(const ParserTools::ExprNode*, void**);
		static int WINAPI QuerySysMetric(int);
		static UCString WINAPI QuerySysMetricStr(int, UString, unsigned int);
		void ReturnValueParser(ParserTools::ValueParser*);
		void SendParseError(UCString, UCString, int, int, long);
		void SendParseError(UCString, UCString, IXmlReader*, long);
		void _LeaveOnCurrentThread();

		long _RecordElementLayout(IXmlReader*, UCString);
		long _RecordElementStyleSheet(UCString, bool);
		long _RecordElementTrees(IXmlReader*);
		long _RecordElementWithChildren(IXmlReader*, bool, UString*);
		long _RecordInstantiateElement(IXmlReader*, UString*);
		long _RecordSetElementProperties(IXmlReader*);
		long _RecordSetValue(IXmlReader*, UCString, UCString);

		static FunctionDefinition<unsigned long> const* __ptr32 const s_fdClr;
		static FunctionDefinition<Value*> const* __ptr32 const s_fdFill;
		static FunctionDefinition<Value*> const* __ptr32 const s_fdGraphic;
		static FunctionDefinition<int> const* __ptr32 const s_fdInt;
		static FunctionDefinition<RECT> const* __ptr32 const s_fdRect;
		static FunctionDefinition<Value*> const* __ptr32 const s_fdString;
	
	private:
		void SetParseState(DUI_PARSE_STATE);
	};

	class UILIB_API DUIFactory
	{
	public:
		DUIFactory(HWND);
		~DUIFactory(void);
		DUIFactory & operator=(DUIFactory const &);
		long CreateParser(void);
		DUIXmlParser * DetachParser(void);
		DUIXmlParser * GetParser(void);
		long LoadFromBuffer(UCString, UINT_PTR, UCString, Element *, unsigned long *, Element * *);
		long LoadFromFile(UCString, UCString, Element *, unsigned long *, Element * *);
		long LoadFromResource(HINSTANCE, UCString, UCString, Element *, unsigned long *, Element * *, UCString);
		void __cdecl SetError(UCString, ...);

	private:
		static void __stdcall s_XMLParseError(UCString, UCString, int, void *);
		void ClearParser(void);
	};

}