#pragma once

namespace DirectUI
{
	class UILIB_API FontCheckOut
	{
	public:
		FontCheckOut(Element *, HDC);
		~FontCheckOut(void);
		FontCheckOut & operator=(FontCheckOut const &);
	};
}