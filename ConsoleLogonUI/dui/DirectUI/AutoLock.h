#pragma once

namespace DirectUI
{
	class UILIB_API AutoLock
	{
	public:
		AutoLock(LPCRITICAL_SECTION lpCriticalSection);
		AutoLock& operator=(const AutoLock& );
		~AutoLock();
	};
}