#pragma once

namespace DirectUI
{
	class UILIB_API DuiNavigate
	{
	public:
		static Element * __stdcall Navigate(Element *, DynamicArray<Element *, 0> *, int);
		DuiNavigate & operator=(DuiNavigate const &);
	};
}