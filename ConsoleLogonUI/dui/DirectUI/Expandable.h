#pragma once

namespace DirectUI
{
	class UILIB_API Expandable :public Element
	{
	public:
		Expandable(Expandable const &);
		Expandable(void);
		virtual ~Expandable(void);
		Expandable & operator=(Expandable const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static const PropertyInfo* __stdcall ExpandedProp(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		bool GetExpanded(void);
		long SetExpanded(bool);

		virtual IClassInfo * GetClassInfoW(void);

	private:
		static IClassInfo * s_pClassInfo;

	};
}