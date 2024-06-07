#pragma once

namespace DirectUI
{
	class UILIB_API AutoThread
	{
	public:
		AutoThread();
		~AutoThread();
		AutoThread& operator=(const AutoThread &);
		HRESULT Init();
	};
}