#pragma once

namespace DirectUI
{
	class UILIB_API CCVScrollBar : public CCBaseScrollBar
	{
	public:
		CCVScrollBar(CCVScrollBar const &);
		CCVScrollBar(void);
		virtual ~CCVScrollBar(void);
		CCVScrollBar & operator=(CCVScrollBar const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);

	private:
		static IClassInfo * s_pClassInfo;

	};
}