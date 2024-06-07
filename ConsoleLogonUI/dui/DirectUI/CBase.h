#pragma once

namespace DirectUI
{
	class UILIB_API CCBase :public HWNDHost
	{
	public:
		CCBase(const CCBase &);
		CCBase(unsigned long v1 =0, UCString v2=NULL);
		CCBase & operator=(const CCBase &);

		virtual ~CCBase(void);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static PropertyInfo const * __stdcall WinStyleProp(void);

		int GetWinStyle(void);
		long Initialize(unsigned int, Element *, unsigned long *);
		void SetNotifyHandler(BOOL (WINAPI*)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult, void* pUserData), void* pUserData);
		long SetWinStyle(int);

		//父类父函数重载
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual HWND CreateHWND(HWND);
		virtual long DefaultAction(void);
		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual void OnInput(InputEvent *);
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

		//1
		virtual bool OnCustomDraw(NMCUSTOMDRAW *, LRESULT *);
		//2
		virtual bool OnLostDialogFocus(DialogElement *);
		//3
		virtual bool OnReceivedDialogFocus(DialogElement *);

	protected:
		//4
		virtual void PostCreate(HWND);
	private:
		static IClassInfo * s_pClassInfo;
	};
}