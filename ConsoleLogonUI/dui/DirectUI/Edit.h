#pragma once

namespace DirectUI
{
	class UILIB_API Edit : public HWNDHost
	{
	public:
		Edit(Edit const &);
		Edit(void);
		virtual ~Edit(void);
		Edit & operator=(Edit const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static const PropertyInfo* __stdcall DirtyProp(void);
		static UID __stdcall Enter(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static const PropertyInfo* __stdcall MultilineProp(void);
		static const PropertyInfo* __stdcall MaxLengthProp(void);
		static const PropertyInfo* __stdcall PasswordCharacterProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static const PropertyInfo* __stdcall ThemedBorderProp(void);
		static const PropertyInfo* __stdcall WantTabsProp(void);
		
		bool GetDirty(void);
		int GetMaxLength(void);
		bool GetMultiline(void);
		int GetPasswordCharacter(void);
		bool GetThemedBorder(void);
		bool GetWantTabs(void);
		long Initialize(unsigned int, Element *, unsigned long *);
		long SetDirty(bool);
		long SetMaxLength(int);
		long SetMultiline(bool);
		long SetPasswordCharacter(int);
		long SetThemedBorder(bool);
		long SetWantTabs(bool);


		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual unsigned short const * GetContentStringAsDisplayed(Value * *);
		virtual bool IsContentProtected(void);
		virtual unsigned int MessageCallback(LPGMSG);
		virtual void OnInput(InputEvent *);
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);

	protected:
		virtual HWND CreateHWND(HWND);

		//1
		virtual HWND CreateHWND(HWND, bool);
	
	private:
		static IClassInfo * s_pClassInfo;
		unsigned int GetTextHeight(void);
		void PropertyChangedCore(const PropertyInfo*, int, Value *, HWND);

	};
}