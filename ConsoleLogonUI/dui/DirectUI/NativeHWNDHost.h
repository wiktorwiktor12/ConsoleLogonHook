#pragma once

namespace DirectUI
{

	class UILIB_API NativeHWNDHost
	{
	public:
		NativeHWNDHost(const NativeHWNDHost&);
		NativeHWNDHost();
		virtual ~NativeHWNDHost();

		static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
		NativeHWNDHost& operator=(const NativeHWNDHost&);
		static UINT WINAPI AsyncDestroyMsg();

		static HRESULT WINAPI Create(UCString lpClassName, UCString lpWindowName, HWND hWndParent, HICON hIcon, int x, int y, int nWidth, int nHeight, int dwExStyle, int dwStyle, HINSTANCE, UINT, NativeHWNDHost**out);
		static HRESULT WINAPI Create(UCString lpWindowName, HWND hWndParent, HICON hIcon, int x, int y, int nWidth, int nHeight, int dwExStyle, int dwStyle, unsigned int, NativeHWNDHost**out);

		void Destroy();
		void DestroyWindow();

		class Element* GetElement();
		HWND GetHWND();

		void HideWindow();
		void Host(class Element*);

		HRESULT Initialize(UCString lpClassName, UCString lpWindowName, HWND hWndParent, HICON hIcon, int x, int y, int nWidth, int nHeight, int dwExStyle, int dwStyle, HINSTANCE, UINT);
		HRESULT Initialize(UCString, HWND, HICON, int, int, int, int, int, int, UINT);

		virtual HRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);

		int RestoreFocus();
		void SaveFocus();
		void SetDefaultFocusID(UCString);
		void ShowWindow(int);

	private:
		HWND m_hWnd;
		void*unk2;
		void*unk3;
		void*unk4;
		char unk5;
	};
}