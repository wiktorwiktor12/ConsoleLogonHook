#pragma once

namespace DirectUI
{
	class UILIB_API XHost
	{
	public:

		XHost();
		~XHost();
		XHost & operator=(XHost const &);

		static long WINAPI Create(class IXElementCP*, XHost**);
		void Destroy();
		void DestroyWindow();

		class Element* GetElement();
		HWND GetHWND();

		void HideWindow();
		void Host(class Element*);
		long Initialize(class IXElementCP*);
		void ShowWindow(int);

		static LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}