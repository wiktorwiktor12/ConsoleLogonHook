#pragma once

namespace DirectUI
{
	class UILIB_API UnknownElement : public Element
	{
	public:
		UnknownElement(UnknownElement const &);
		UnknownElement(void);
		virtual ~UnknownElement(void);
		UnknownElement & operator=(UnknownElement const &);
		
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
	private:
		static IClassInfo * s_pClassInfo;

	};
}