#pragma once

namespace DirectUI
{
	class UILIB_API XElement
		: public HWNDHost
		, public IXElementCP
	{
	public:
		XElement(XElement const &);
		XElement(void);
		virtual ~XElement(void);
		XElement & operator=(XElement const &);
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static unsigned int const s_uButtonFocusChangeMsg;
		static unsigned int const s_uNavigateOutMsg;
		static unsigned int const s_uUnhandledSyscharMsg;
		static UID __stdcall UnhandledSyschar(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		void FreeProvider(void);
		HWND GetInnerHWND(void);
		struct IXProvider * GetProvider(void);
		long Initialize(unsigned int, Element *, unsigned long *);
		bool IsDescendent(Element *);
		long SetProvider(IUnknown *);

		virtual HWND CreateHWND(HWND);
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnInput(InputEvent *);
		virtual bool OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);
		virtual bool OnSinkThemeChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);
		virtual bool OnSysChar(unsigned short);
		virtual void SetKeyFocus(void);

		//IXElementCP
		virtual HWND GetNotificationSinkHWND(void);

	private:
		static IClassInfo * s_pClassInfo;
	};
}