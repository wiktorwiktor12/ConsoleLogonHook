#pragma once

namespace DirectUI
{
	class UILIB_API ElementWithHWND : public Element
	{
	public:
		ElementWithHWND(const ElementWithHWND &);
		ElementWithHWND();
		ElementWithHWND& operator=(const ElementWithHWND &);

		virtual ~ElementWithHWND();

		static long WINAPI Create(Element*, unsigned long*, Element**);
		static IClassInfo* WINAPI GetClassInfoPtr();
		static long WINAPI Register();
		static void WINAPI SetClassInfoPtr(IClassInfo*);

		virtual IClassInfo* GetClassInfoW();

	private:
		static IClassInfo*s_pClassInfo;
	};
}