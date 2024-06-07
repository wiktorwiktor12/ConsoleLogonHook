#pragma once

namespace DirectUI
{
	class UILIB_API XBaby : public DialogElement
	{
	public:
		XBaby(XBaby const &);
		XBaby(void);
		virtual ~XBaby(void);
		XBaby & operator=(XBaby const &);
		void CacheParser(DUIXmlParser *);
		
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Create(IXElementCP *,class XProvider *, HWND, Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		void ForceThemeChange(UINT_PTR, LONG_PTR);
		SIZE GetContentDesiredSize(int, int);
		long GetHostedElementID(unsigned short *);
		long SetToHost(Element *);

		virtual bool CanSetFocus(void);
		virtual long CreateStyleParser(DUIXmlParser * *);
		virtual Element * GetAdjacent(Element *, int, NavReference const *, unsigned long);
		virtual IClassInfo * GetClassInfoW(void);
		virtual bool OnChildLostFocus(Element *);
		virtual bool OnChildReceivedFocus(Element *);
		virtual void OnEvent(Event *);
		virtual void OnNoChildWithShortcutFound(KeyboardEvent *);
		virtual void OnThemeChanged(ThemeChangedEvent *);
		virtual void OnWmThemeChanged(WPARAM wParam, LPARAM lParam);
		virtual void SetKeyFocus(void);

		//1
		virtual Element * GetFocusableElement(void);
	protected:
		long Initialize(IXElementCP *, XProvider *, HWND, Element *, unsigned long *);
	private:
		static IClassInfo * s_pClassInfo;
	};
}