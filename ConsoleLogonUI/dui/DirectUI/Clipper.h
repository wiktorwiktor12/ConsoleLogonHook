#pragma once

namespace DirectUI
{
	class UILIB_API Clipper : public Expandable
	{
	public:
		Clipper(Clipper const &);
		Clipper(void);
		virtual ~Clipper(void);
		Clipper & operator=(Clipper const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void _SelfLayoutDoLayout(int, int);
		virtual SIZE _SelfLayoutUpdateDesiredSize(int, int, Surface *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}