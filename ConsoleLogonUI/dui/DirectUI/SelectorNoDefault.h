#pragma once

namespace DirectUI
{
	class UILIB_API SelectorNoDefault :public Selector
	{
	public:
		SelectorNoDefault(SelectorNoDefault const &);
		SelectorNoDefault(void);
		virtual ~SelectorNoDefault(void);
		SelectorNoDefault & operator=(SelectorNoDefault const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(Element *, unsigned long *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnKeyFocusMoved(Element *, Element *);
		virtual long SetSelection(Element *);
	private:
		static IClassInfo * s_pClassInfo;

	};
}