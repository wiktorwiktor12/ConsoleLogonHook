#pragma once

namespace DirectUI
{
	class UILIB_API TextGraphic : public Element
	{
	public:
		TextGraphic(TextGraphic const &);
		TextGraphic(void);
		virtual ~TextGraphic(void);
		TextGraphic & operator=(TextGraphic const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static PropertyInfo const * __stdcall SideGraphicProp(void);

		long Initialize(Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual unsigned short const * GetContentStringAsDisplayed(Value * *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual bool OnPropertyChanging(PropertyInfo const *, int, Value *, Value *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}