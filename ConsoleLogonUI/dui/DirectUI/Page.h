#pragma once

namespace DirectUI
{
	class UILIB_API Page : public Macro
	{
	public:
		Page(Page const &);
		Page(void);
		virtual ~Page(void);
		Page & operator=(Page const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(Element *, unsigned long *);
		
		virtual IClassInfo * GetClassInfoW(void);

	private:
		static IClassInfo * s_pClassInfo;

	};

	class UILIB_API Pages : public Element
	{
	public:
		Pages(Pages const &);
		Pages(void);
		virtual ~Pages(void);
		Pages & operator=(Pages const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		unsigned int GetCount(void);
		Element * GetPage(unsigned int);
		Element * GetPage(unsigned short const *);
		long Initialize(Element *, unsigned long *);
		void SetNoBrowseOnFirstAdd(void);

		virtual long Add(Element * *, unsigned int);
		virtual IClassInfo * GetClassInfoW(void);

	private:
		static IClassInfo * s_pClassInfo;

	};
}