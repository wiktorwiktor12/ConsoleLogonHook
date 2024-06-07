#pragma once

namespace DirectUI
{
	class UILIB_API Browser :public Element
	{
	public:
		Browser(const Browser &);
		Browser(void);
		Browser & operator=(const Browser &);
		virtual ~Browser(void);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static UID __stdcall Entered(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static UID __stdcall Leaving(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static UID __stdcall StartNavigate(void);

		Element * GetCurrentPage(void);
		unsigned short GetCurrentPageID(void);
		class Pages * GetPages(void);
		long Initialize(Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnPropertyChanged(const PropertyInfo *, int, Value *, Value *);
	private:
		int FireNavigate(unsigned short);
		static IClassInfo * s_pClassInfo;
	};
}