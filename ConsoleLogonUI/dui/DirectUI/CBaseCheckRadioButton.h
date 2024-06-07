#pragma once

namespace DirectUI
{
	//此类无新增虚函数
	class UILIB_API CCBaseCheckRadioButton : public CCPushButton
	{
	public:
		CCBaseCheckRadioButton(const CCBaseCheckRadioButton &);
		CCBaseCheckRadioButton(unsigned long);
		CCBaseCheckRadioButton & operator=(const CCBaseCheckRadioButton &);

		virtual ~CCBaseCheckRadioButton(void);

		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual bool OnLostDialogFocus(DialogElement *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual bool OnReceivedDialogFocus(DialogElement *);
	protected:
		virtual void PostCreate(HWND);

	private:
		static IClassInfo * s_pClassInfo;
	};
}