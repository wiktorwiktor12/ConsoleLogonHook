#pragma once

namespace DirectUI
{
	class UILIB_API ScrollBar
		: public Element
		, public BaseScrollBar
	{
	public:
		ScrollBar(ScrollBar const &);
		ScrollBar(void);
		virtual ~ScrollBar(void);
		ScrollBar & operator=(ScrollBar const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Create(bool, Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static PropertyInfo const * __stdcall LineProp(void);
		static PropertyInfo const * __stdcall MaximumProp(void);
		static PropertyInfo const * __stdcall MinimumProp(void);
		static PropertyInfo const * __stdcall OrderProp(void);
		static PropertyInfo const * __stdcall PageProp(void);
		static PropertyInfo const * __stdcall PositionProp(void);
		static PropertyInfo const * __stdcall ProportionalProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static PropertyInfo const * __stdcall VerticalProp(void);
		
		int GetOrder(void);
		bool GetVertical(void);
		long Initialize(bool, Element *, unsigned long *);
		long SetOrder(int);
		long SetProportional(bool);
		long SetVertical(bool);

		virtual IClassInfo * GetClassInfoW(void);
		virtual Element * GetElement(void);
		virtual int GetLine(void);
		virtual int GetMaximum(void);
		virtual int GetMinimum(void);
		virtual int GetPage(void);
		virtual int GetPosition(void);
		virtual bool GetProportional(void);
		virtual void OnEvent(Event *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual bool OnPropertyChanging(PropertyInfo const *, int, Value *, Value *);
		virtual long SetLine(int);
		virtual long SetMaximum(int);
		virtual long SetMinimum(int);
		virtual long SetPage(int);
		virtual long SetPosition(int);
		virtual void _SelfLayoutDoLayout(int, int);
		virtual SIZE _SelfLayoutUpdateDesiredSize(int, int, Surface *);
	protected:
		//1
		virtual long CreateButtons(void);
	private:
		static IClassInfo * s_pClassInfo;

	};

	class UILIB_API ScrollBarRangeValueProxy : public IProxy
	{
	public:
		ScrollBarRangeValueProxy(ScrollBarRangeValueProxy const &);
		ScrollBarRangeValueProxy(void);
		ScrollBarRangeValueProxy & operator=(ScrollBarRangeValueProxy const &);

		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);

	};
}