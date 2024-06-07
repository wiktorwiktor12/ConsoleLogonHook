#pragma once

namespace DirectUI
{
	class UILIB_API Viewer : public Element
	{
	public:
		Viewer(Viewer const &);
		Viewer(void);
		virtual ~Viewer(void);
		Viewer & operator=(Viewer const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static PropertyInfo const * __stdcall XOffsetProp(void);
		static PropertyInfo const * __stdcall XScrollableProp(void);
		static PropertyInfo const * __stdcall YOffsetProp(void);
		static PropertyInfo const * __stdcall YScrollableProp(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);

		int GetXOffset(void);
		bool GetXScrollable(void);
		int GetYOffset(void);
		bool GetYScrollable(void);
		long Initialize(Element *, unsigned long *);
		long SetXOffset(int);
		long SetXScrollable(bool);
		long SetYOffset(int);
		long SetYScrollable(bool);

		virtual bool EnsureVisible(int, int, int, int);
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnInput(InputEvent *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual bool OnPropertyChanging(PropertyInfo const *, int, Value *, Value *);
		virtual void _SelfLayoutDoLayout(int, int);
		virtual SIZE _SelfLayoutUpdateDesiredSize(int, int, Surface *);

	private:
		static IClassInfo * s_pClassInfo;
		Element * GetContent(void);
		bool InternalEnsureVisible(int, int, int, int);

	};
}