#pragma once

namespace DirectUI
{
	class UILIB_API HWNDHost : public ElementWithHWND
	{
	public:
		HWNDHost(const HWNDHost &);
		HWNDHost();
		HWNDHost& operator=(const HWNDHost &);
		
		virtual ~HWNDHost();

		long Initialize(unsigned int, unsigned int, Element*, unsigned long*);
		void Detach();
		unsigned short GetBackgroundOwnerID();
		HWND GetHWNDParent();
		bool GetOptimizeMove();
		bool GetTransparent();
		long GetClientAccessibleImpl(IAccessible**);
		long SetBackgroundOwnerID(UCString);
		long SetOptimizeMove(bool);
		long SetTransparent(bool);

		static long WINAPI Create(unsigned int, unsigned int, Element*, unsigned long*, Element**pOut);
		static long WINAPI Create(Element*, unsigned long*, Element**pOut);
		static const PropertyInfo* WINAPI BackgroundOwnerIDProp();
		static IClassInfo* WINAPI GetClassInfoPtr();
		static const PropertyInfo* WINAPI OptimizeMoveProp();
		static long WINAPI Register();
		static void WINAPI SetClassInfoPtr(IClassInfo*);
		static const PropertyInfo* WINAPI ThemeChangedProp();
		static const PropertyInfo* WINAPI TransparentProp();

		//父类重载
		virtual IClassInfo* GetClassInfoW();
		virtual long GetAccessibleImpl(IAccessible**);
		virtual bool GetKeyFocused();
		virtual unsigned int MessageCallback(LPGMSG);
		virtual void OnDestroy();
		virtual void OnEvent(Event*);
		virtual void OnInput(InputEvent*);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value*, Value*);

		virtual void Paint(HDC, LPCRECT, LPCRECT, LPRECT, LPRECT);
		virtual void SetKeyFocus();
		virtual void SetWindowDirection(HWND);
		//

		//1
		virtual HWND GetHWND();
		//2
		virtual bool OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);
		//3
		virtual bool OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);
		//4
		virtual bool OnSysChar(UChar);
		//5
		virtual bool OnSinkThemeChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT*);
		//6
		virtual bool OnCtrlThemeChanged(UINT, WPARAM wParam, LPARAM lParam, LRESULT*);
		//7
		virtual void OnWindowStyleChanged(WPARAM, const STYLESTRUCT *);
		//8
		virtual int OnAdjustWindowSize(int, int, unsigned int);

	protected:
		static void WINAPI AttachCtrlSubclassProc(HWND);
		static LRESULT WINAPI CtrlSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void PrintRTLControl(HDC, HDC, const RECT&);
		long SetThemeChanged(int);
		void SyncBackground();
		void SyncDirection();
		void SyncFont();
		void SyncForeground();
		void SyncParent();
		void SyncRect(unsigned int, bool);
		void SyncStyle();
		void SyncText();
		void SyncVisible();
		int VerifyParentage();
		HWND CreateAccNameLabel(HWND);
		Element* GetBackgroundOwner();
		HFONT GetFont();
		bool GetStaticColor(HDC, HBRUSH*);
		int GetThemeChanged();
		bool IsMoveDeferred();
		//父类重载
		virtual void OnHosted(Element*);
		virtual void OnUnHosted(Element*);
		//

		//9
		virtual HWND CreateHWND(HWND);
		//10
		virtual bool EraseBkgnd(HDC, LRESULT*);

	private:
		static unsigned int const (* __ptr32 g_rgMouseMap)[3];
		static IClassInfo* s_pClassInfo;
		static BOOL __stdcall _CtrlWndProc(void *, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		static BOOL WINAPI _SinkWndProc(void *, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

		void ApplySinkRegion(RECT const *, bool);
		long GetAccessibleImpl(IAccessible**, bool);
		void GetSinkRect(RECT const *, LPRECT);
		bool HaveWin32Focus();
		void SyncColorsAndFonts();
		void UnvirtualizePosition();
		void _DeleteCtrlWnd();
	};
}