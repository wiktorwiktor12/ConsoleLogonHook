#pragma once

namespace DirectUI
{

	class UILIB_API RepeatButton : public Button
	{
	public:
		RepeatButton(RepeatButton const &);
		RepeatButton(void);
		virtual ~RepeatButton(void);
		RepeatButton & operator=(RepeatButton const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnInput(struct InputEvent *);
	private:
		static void __stdcall _RepeatButtonActionCallback(struct GMA_ACTIONINFO *);
		static IClassInfo * s_pClassInfo;

	};
}