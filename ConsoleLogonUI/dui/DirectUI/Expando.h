#pragma once

namespace DirectUI
{
	class UILIB_API Expando : public Expandable
	{
	public:
		Expando(Expando const &);
		Expando(void);
		virtual ~Expando(void);
		Expando & operator=(Expando const &);

		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);

		long Initialize(Element *, unsigned long *);

		virtual long Add(Element * *, unsigned int);
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnEvent(Event *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
	protected:
		static unsigned short __stdcall Arrow(void);
		static unsigned short __stdcall Clipper(void);
		void UpdateChildren(Value *);

	private:
		static unsigned short _atmArrow;
		static unsigned short _atmClipper;
		static IClassInfo * s_pClassInfo;
	};
}