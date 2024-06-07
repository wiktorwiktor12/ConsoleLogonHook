#pragma once

namespace DirectUI
{
	class UILIB_API CCSysLink :public CCBase
	{
	public:
		CCSysLink(CCSysLink const &);
		CCSysLink(void);
		virtual ~CCSysLink(void);
		CCSysLink & operator=(CCSysLink const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual struct tagSIZE GetContentSize(int, int, Surface *);
		virtual void OnInput(InputEvent *);
		virtual bool OnLostDialogFocus(DialogElement *);
		virtual bool OnReceivedDialogFocus(DialogElement *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}