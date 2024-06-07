#pragma once

namespace DirectUI
{
	class UILIB_API CCListBox : public CCBase
	{
	public:
		CCListBox(CCListBox const &);
		CCListBox(void);
		virtual ~CCListBox(void);
		CCListBox & operator=(CCListBox const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		int DeleteString(int);
		int AddString(unsigned short const *);
		int GetCount(void);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}