#pragma once

namespace DirectUI
{
	class UILIB_API PText : public TextGraphic
	{
	public:
		PText(PText const &);
		PText(void);
		virtual ~PText(void);
		PText & operator=(PText const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		long Initialize(Element *, unsigned long *);
		void SetDataEntry(IDataEntry *);

		virtual IClassInfo * GetClassInfoW(void);
		virtual bool OnPropertyChanging(PropertyInfo const *, int, Value *, Value *);

	private:
		static IClassInfo * s_pClassInfo;
	};
}