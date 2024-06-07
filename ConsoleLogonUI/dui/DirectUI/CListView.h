#pragma once

namespace DirectUI
{
	class UILIB_API CCListView : public CCBase
	{
	public:
		CCListView(CCListView const &);
		CCListView(void);
		virtual ~CCListView(void);
		CCListView & operator=(CCListView const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
	private:
		static IClassInfo * s_pClassInfo;
	};
}