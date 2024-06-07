#pragma once

namespace DirectUI
{
	class UILIB_API Selector : public Element
	{
	public:
		Selector(Selector const &);
		Selector(void);
		virtual ~Selector(void);
		Selector & operator=(Selector const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Register(void);
		static UID __stdcall SelectionChange(void);
		static PropertyInfo const * __stdcall SelectionProp(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);

		Element * GetSelection(void);
		long Initialize(Element *, unsigned long *);

		virtual Element * GetAdjacent(Element *, int, NavReference const *, unsigned long);
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnInput(struct InputEvent *);
		virtual void OnKeyFocusMoved(Element *, Element *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		
		//1
		virtual long SetSelection(Element *);

	private:
		static IClassInfo * s_pClassInfo;

	};
}