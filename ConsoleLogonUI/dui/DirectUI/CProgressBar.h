#pragma once

namespace DirectUI
{
	class UILIB_API CCProgressBar :public CCBase
	{
	public:
		CCProgressBar(CCProgressBar const &);
		CCProgressBar(void);
		virtual ~CCProgressBar(void);
		CCProgressBar & operator=(CCProgressBar const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnInput(InputEvent *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}