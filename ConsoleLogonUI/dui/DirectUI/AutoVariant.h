#pragma once
#include "types.h"

namespace DirectUI
{
	class UILIB_API AutoVariant
	{
	public:
		AutoVariant();
		AutoVariant& operator=(const AutoVariant &);
		~AutoVariant();
	};
}