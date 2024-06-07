#pragma once

namespace DirectUI
{
	//32位结构体大小 0x94
	class UILIB_API HWNDElement : public ElementWithHWND
	{
	public:
		HWNDElement(const HWNDElement &);
		HWNDElement();
		virtual ~HWNDElement();
		HWNDElement& operator=(const HWNDElement &);

		//0
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value*, Value*);
		//1
		virtual void OnGroupChanged(int, bool);
		//2
		virtual void OnInput(InputEvent*);
		//3
		virtual void OnDestroy();
		//4
		virtual void OnEvent(Event*);
		//5
		virtual void UpdateTooltip(Element*);

		//6
		virtual void ActivateTooltip(Element*, unsigned long);
		//7
		virtual void RemoveTooltip(Element*);

		//8
		virtual IClassInfo* GetClassInfoW();
		//9
		virtual long GetAccessibleImpl(IAccessible**);


		//HWNDElement 新增函数
		//10
		virtual HWND GetHWND();

		//11
		virtual void OnThemeChanged(ThemeChangedEvent*);

		//12
		virtual void OnNoChildWithShortcutFound(KeyboardEvent*);

		//13
		virtual void OnGetDlgCode(LPMSG, LRESULT*);
		//14
		virtual void OnWmThemeChanged(WPARAM wParam, LPARAM lParam);
		//15
		virtual void OnCompositionChanged();

		//16
		virtual bool CanSetFocus();

		static UID WINAPI CompositionChange();
		static HRESULT WINAPI Create(HWND, bool, unsigned int, Element*, unsigned long*, Element**pOut);
		//17
		virtual long CreateStyleParser(DUIXmlParser**);
		void DelayActivateTooltip();
		Element* ElementFromPoint(LPPOINT);
		static bool WINAPI FindShortcut(unsigned short, Element*, Element**, int*, int*, int);
		void FlushWorkingSet();
		static IClassInfo* WINAPI GetClassInfoPtr();

		static HWNDElement* WINAPI GetFocusedHWNDElement();

		static Element* WINAPI GetKeyFocusedElement();
		unsigned short GetUIState();
		bool GetWrapKeyboardNavigate();
		long Initialize(HWND, bool, unsigned int, Element*, unsigned long*);
		bool IsFirstElement(Element*);
		bool IsLastElement(Element*);


		static long WINAPI Register();
		static void WINAPI SetClassInfoPtr(IClassInfo*);

		void SetFocus(bool);
		void SetParentSizeControl(bool);
		void SetScreenCenter(bool);
		long SetWrapKeyboardNavigate(bool);
		bool ShowAccel(void);
		bool ShowFocus(void);
		void ShowUIState(bool, bool);
		static UID WINAPI ThemeChange();

		void ToggleUIState(bool, bool);
		static const PropertyInfo* WINAPI WrapKeyboardNavigateProp();

		static LRESULT WINAPI StaticWndProc(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam);
		//18
		virtual LRESULT WndProc(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		static bool WINAPI FindShortcutRecursive(unsigned short, Element*, Element**, int*, int*, int);

	private:
		static IClassInfo* s_pClassInfo;
	};

	class UILIB_API HWNDElementProvider
		: public ElementProvider
		, public IRawElementProviderFragmentRoot
	{
	public:
		HWNDElementProvider(void);
		virtual ~HWNDElementProvider(void);

		HWNDElementProvider& operator=(const HWNDElementProvider&) = delete;

		static long __stdcall Create(HWNDElement *, InvokeHelper *, HWNDElementProvider * *);

		virtual unsigned long __stdcall AddRef(void);
		virtual long __stdcall QueryInterface(struct _GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);

		virtual ProviderProxyCall GetProxyCreator(void);

		//IRawElementProviderFragmentRoot
		//1
		virtual long __stdcall ElementProviderFromPoint(double, double, IRawElementProviderFragment * *);
		//2
		virtual long __stdcall GetFocus(IRawElementProviderFragment * *);

	protected:
		virtual long Init(HWNDElement *, InvokeHelper *);

	};

	//此类存在问题，看虚表3有个，但是导出只有2个，怪异……
	class UILIB_API HWNDElementProxy : public IProxy
	{
	public:
		HWNDElementProxy(HWNDElementProxy const &);
		HWNDElementProxy(void);
		HWNDElementProxy & operator=(HWNDElementProxy const &);

		static HWNDElementProxy * __stdcall Create(HWNDElement *);
		virtual long DoMethod(int, char *);
		virtual void Init(HWNDElement *);

	protected:
		long ElementFromPoint(double, double, IRawElementProviderFragment * *);
		long GetFocus(IRawElementProviderFragment * *);

	};
}