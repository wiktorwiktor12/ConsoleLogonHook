#pragma once

namespace DirectUI
{
	class UILIB_API CCRadioButton : public CCBaseCheckRadioButton
	{
	public:
		CCRadioButton(CCRadioButton const &);
		CCRadioButton(void);
		virtual ~CCRadioButton(void);
		CCRadioButton & operator=(CCRadioButton const &);

		static PropertyInfo const * __stdcall AutoGroupingProp(void);
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);

		bool GetAutoGrouping(void);
		long SetAutoGrouping(bool);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnInput(InputEvent *);
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
	private:
		static IClassInfo * s_pClassInfo;
	};
}