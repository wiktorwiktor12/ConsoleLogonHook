#pragma once

namespace DirectUI
{
	class UILIB_API CCAVI :public CCBase
	{
	public:
		CCAVI(const CCAVI &);
		CCAVI(void);
		CCAVI & operator=(const CCAVI &);

		virtual ~CCAVI(void);
		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		void Play(HWND);
		void Stop(void);
		virtual IClassInfo * GetClassInfoW(void);
	protected:
		virtual void PostCreate(HWND);
	private:
		void OpenAnimation(HWND);

		static IClassInfo * s_pClassInfo;
		
	};
}