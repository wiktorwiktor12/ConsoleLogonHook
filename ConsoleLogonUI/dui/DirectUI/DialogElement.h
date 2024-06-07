#pragma once

namespace DirectUI
{
	class UILIB_API DialogElement
		: public HWNDElement
		, public IElementListener
	{
	public:
		DialogElement(DialogElement const &);
		DialogElement(void);
		virtual ~DialogElement(void);
		DialogElement & operator=(DialogElement const &);

		static const PropertyInfo* __stdcall ButtonClassAcceptsEnterKeyProp(void);
		static long __stdcall Create(struct HWND__ *, bool, unsigned int, Element *, unsigned long *, Element * *);
		static const PropertyInfo* __stdcall DefaultButtonTrackingProp(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static const PropertyInfo* __stdcall HandleEnterKeyProp(void);
		static long __stdcall Register(void);
		static const PropertyInfo* __stdcall RegisteredDefaultButtonProp(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		bool GetButtonClassAcceptsEnterKey(void);
		bool GetDefaultButtonTracking(void);
		bool GetHandleEnterKey(void);
		Element * GetRegisteredDefaultButton(void);
		long SetButtonClassAcceptsEnterKey(bool);
		long SetDefaultButtonTracking(bool);
		long SetHandleEnterKey(bool);
		long SetRegisteredDefaultButton(Element *);

		//基类
		virtual void OnDestroy(void);
		virtual void OnGetDlgCode(MSG *, LRESULT *);
		virtual void OnInput(InputEvent *);
		virtual void OnKeyFocusMoved(Element *, Element *);
		virtual void OnListenedEvent(Element *, Event *);
		virtual void OnListenedInput(Element *, InputEvent *);
		virtual void OnListenedPropertyChanged(Element *, const PropertyInfo*, int, Value *, Value *);
		virtual bool OnListenedPropertyChanging(Element *, const PropertyInfo*, int, Value *, Value *);
		virtual void OnListenerAttach(Element *);
		virtual void OnListenerDetach(Element *);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);
		virtual IClassInfo * GetClassInfoW(void);

		//1
		virtual Element * GetDefaultButton(void);
		//2
		virtual bool ClickDefaultButton(void);
		//3
		virtual void UpdateChildFocus(Element *, Element *);
		//4
		virtual bool OnChildLostFocus(Element *);
		//5
		virtual bool OnChildReceivedFocus(Element *);

	protected:
		void SetRegisteredDefaultButtonSelectedState(bool);
		static bool __stdcall IsButtonEnabledAndVisible(Element *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}