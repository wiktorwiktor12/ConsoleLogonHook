#pragma once

namespace DirectUI
{
	class UILIB_API RefPointElement : public Element
	{
	public:
		RefPointElement(RefPointElement const &);
		RefPointElement(void);
		virtual ~RefPointElement(void);
		RefPointElement & operator=(RefPointElement const &);

		static PropertyInfo const * __stdcall ActualReferencePointProp(void);
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static Element * __stdcall FindRefPoint(Element *, POINT *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static PropertyInfo const * __stdcall ReferencePointProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static RefPointElement * __stdcall Locate(Element *);
		
		POINT const * GetActualReferencePoint(Value * *);
		POINT const * GetReferencePoint(Value * *);
		long Initialize(unsigned int, Element *, unsigned long *);
		long SetReferencePoint(int, int);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
	private:
		static IClassInfo * s_pClassInfo;

	};
}