#pragma once

namespace DirectUI
{
	class UILIB_API Button : public Element
	{
	public:
		Button(const Button&);
		Button();
		virtual ~Button();
		Button & operator=(const Button&);

		long Initialize(unsigned int, Element*, unsigned long*);
		static HRESULT WINAPI Create(unsigned int, Element*, unsigned long*, Element**);
		static HRESULT WINAPI Create(Element*, unsigned long*, Element**);

		static UID WINAPI Click();
		static UID WINAPI Context();
		//Element类函数重写
		//0
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value*, Value*);
		//1
		virtual void OnInput(InputEvent*);

		//2
		virtual IClassInfo* GetClassInfoW();

		//3
		virtual long DefaultAction();

		bool GetCaptured();
		static IClassInfo* WINAPI GetClassInfoPtr();
		bool GetPressed();
		//Button 新增虚函数
		//0
		virtual bool OnLostDialogFocus(class DialogElement*);
		//1
		virtual bool OnReceivedDialogFocus(class DialogElement*);

		static long WINAPI Register();
		static const PropertyInfo* WINAPI CapturedProp();
		static const PropertyInfo* WINAPI PressedProp();
		static void WINAPI SetClassInfoPtr(IClassInfo*);
		
		long SetCaptured(bool);
		long SetPressed(bool);


	private:
		static IClassInfo* s_pClassInfo;
	};
}