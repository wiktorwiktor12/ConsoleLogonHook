#pragma once

namespace DirectUI
{
	//此类虚函数全部来自父
	class UILIB_API CCHScrollBar : public CCBaseScrollBar
	{
	public:
		CCHScrollBar(const CCHScrollBar &);
		CCHScrollBar(void);
		CCHScrollBar & operator=(const CCHScrollBar &);
		virtual ~CCHScrollBar(void);
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