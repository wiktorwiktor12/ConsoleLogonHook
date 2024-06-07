#pragma once

namespace DirectUI
{
	//所有虚函数均是重载，无需调整顺序
	class UILIB_API AutoButton :public AccessibleButton
	{
	public:
		AutoButton(const AutoButton &);
		AutoButton();
		AutoButton & operator=(const AutoButton&);
		virtual ~AutoButton();

		static long __stdcall Create(Element* , unsigned long *, Element** );
		static IClassInfo* __stdcall GetClassInfoPtr();
		static long __stdcall Register();
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static UID __stdcall Toggle();

		virtual IClassInfo* GetClassInfoW();
		virtual void OnEvent(Event *);

		long Initialize(Element *, unsigned long *);

	private:
		static IClassInfo* s_pClassInfo;
	};
}