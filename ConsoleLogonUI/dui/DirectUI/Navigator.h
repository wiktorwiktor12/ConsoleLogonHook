#pragma once

namespace DirectUI
{
	class UILIB_API Navigator : public PushButton
	{
	public:
		Navigator(Navigator const &);
		Navigator(void);
		virtual ~Navigator(void);
		Navigator & operator=(Navigator const &);

		static UID __stdcall ActionInitiated(void);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static PropertyInfo const * __stdcall TargetPageProp(void);

		Browser * GetBrowser(void);
		unsigned short const * GetTargetPage(Value * *);
		long Initialize(Element *, unsigned long *);
		long SetTargetPage(unsigned short const *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);

	private:
		static IClassInfo * s_pClassInfo;
		void FireNavigationEvent(void);

	};

	class UILIB_API NavigatorSelectionItemProxy : public IProxy
	{
	public:
		NavigatorSelectionItemProxy(NavigatorSelectionItemProxy const &);
		NavigatorSelectionItemProxy(void);
		NavigatorSelectionItemProxy & operator=(NavigatorSelectionItemProxy const &);
		//1
		virtual long DoMethod(int, char *);
	protected:
		//2
		virtual void Init(Element *);

	private:
		long AddToSelection(Browser *);
		long GetIsSelected(Browser *, int *);

	};
}