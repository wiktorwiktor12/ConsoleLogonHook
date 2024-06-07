#pragma once

namespace DirectUI
{
	class UILIB_API CheckBoxGlyph : public Button
	{
	public:
		CheckBoxGlyph(CheckBoxGlyph const &);
		CheckBoxGlyph(void);
		virtual ~CheckBoxGlyph(void);
		CheckBoxGlyph & operator=(CheckBoxGlyph const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(unsigned int, Element *, unsigned long *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual bool OnLostDialogFocus(DialogElement *);
		virtual bool OnReceivedDialogFocus(DialogElement *);

	private:
		static IClassInfo * s_pClassInfo;

	};
}