#pragma once

namespace DirectUI
{
	class UILIB_API Repeater : public Macro
	{
	public:
		Repeater(Repeater const &);
		Repeater(void);
		virtual ~Repeater(void);
		Repeater & operator=(Repeater const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		long Initialize(Element *, unsigned long *);
		void SetDataEngine(IDataEngine *);

		virtual IClassInfo * GetClassInfoW(void);
	protected:
		virtual long BuildElement(void);
	private:
		static IClassInfo * s_pClassInfo;

	};
}