#pragma once

namespace DirectUI
{
	class UILIB_API FontCache
	{
	public:
		FontCache(FontCache const &);
		FontCache(void);
		FontCache & operator=(FontCache const &);

		static long __stdcall InitProcess(void);
		static long __stdcall InitThread(void);
		static void __stdcall UninitProcess(void);
		static void __stdcall UninitThread(void);

		virtual void T1() = 0;
		virtual void T2() = 0;
		virtual void T3() = 0;
		virtual void T4() = 0;
	};
}