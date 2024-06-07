#pragma once

namespace DirectUI
{
	class UILIB_API Thumb : public Button
	{
	public:
		Thumb(Thumb const &);
		Thumb(void);
		virtual ~Thumb(void);
		Thumb & operator=(Thumb const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static UID __stdcall Drag(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnInput(InputEvent *);
	private:
		static IClassInfo * s_pClassInfo;

	};
}