#pragma once

namespace DirectUI
{
	class UILIB_API Combobox : public HWNDHost
	{
	public:
		Combobox(Combobox const &);
		Combobox(void);
		virtual ~Combobox(void);
		Combobox & operator=(Combobox const &);

		
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static UID __stdcall SelectionChange(void);
		static const PropertyInfo* __stdcall SelectionProp(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		long SetSelection(int);
		int AddString(unsigned short const *);
		int GetSelection(void);
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual HWND CreateHWND(HWND);
		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual int OnAdjustWindowSize(int, int, unsigned int);
		virtual void OnHosted(Element *);
		virtual void OnInput(InputEvent *);
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);

	private:
		static IClassInfo * s_pClassInfo;

	};
}