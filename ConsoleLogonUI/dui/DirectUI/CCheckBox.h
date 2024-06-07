#pragma once

namespace DirectUI
{
	class UILIB_API CCCheckBox :public CCBaseCheckRadioButton
	{
	public:
		CCCheckBox(const CCCheckBox &);
		CCCheckBox(unsigned long v1=0x403);
		CCCheckBox & operator=(const CCCheckBox &);
		virtual ~CCCheckBox(void);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnInput(InputEvent *);
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

	private:
		static IClassInfo * s_pClassInfo;
	};
}