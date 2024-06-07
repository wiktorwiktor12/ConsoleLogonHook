#pragma once

namespace DirectUI
{
	MIDL_INTERFACE("1e3d87cb-f434-4838-a004-f14e385e736f") IXProvider : public IUnknown
	{
	public:
		virtual long __stdcall CreateDUI(IXElementCP*, HWND*) = 0;
		virtual long __stdcall SetParameter(GUID const&, void*) = 0;
		virtual long __stdcall GetDesiredSize(int, int, SIZE*) = 0;
		virtual long __stdcall IsDescendent(Element*, bool*) = 0;
		virtual long __stdcall SetFocus(Element*) = 0;
		virtual long __stdcall Navigate(int, bool*) = 0;
		virtual long __stdcall CanSetFocus(bool*) = 0;
		virtual int __stdcall FindElementWithShortcutAndDoDefaultAction(unsigned short, int) = 0;
		virtual long __stdcall GetHostedElementID(unsigned short*) = 0;
		virtual long __stdcall ForceThemeChange(UINT_PTR, LONG_PTR) = 0;
		virtual long __stdcall SetDefaultButtonTracking(bool) = 0;
		virtual int __stdcall ClickDefaultButton(void) = 0;
		virtual long __stdcall SetRegisteredDefaultButton(Element*) = 0;
		virtual long __stdcall SetButtonClassAcceptsEnterKey(bool) = 0;
	};
	interface IXBaby;
	class UILIB_API XProvider : public IXProvider
	{
	public:
		XProvider(XProvider const&);
		XProvider(void);


		virtual long  CreateDUI(IXElementCP*, HWND*);
		virtual long __stdcall SetParameter(GUID const&, void*);
		virtual long __stdcall GetDesiredSize(int, int, SIZE*);
		virtual long __stdcall IsDescendent(Element*, bool*);
		virtual long __stdcall SetFocus(Element*);
		virtual long __stdcall Navigate(int, bool*);
		virtual long __stdcall CanSetFocus(bool*);
		virtual int __stdcall FindElementWithShortcutAndDoDefaultAction(unsigned short, int);
		virtual long __stdcall GetHostedElementID(unsigned short*);
		virtual long __stdcall ForceThemeChange(UINT_PTR, LONG_PTR);
		virtual long __stdcall SetDefaultButtonTracking(bool);
		virtual int __stdcall ClickDefaultButton(void);
		virtual long __stdcall SetRegisteredDefaultButton(Element*);
		virtual long __stdcall QueryInterface(GUID const&, void**);
		virtual unsigned long __stdcall AddRef(void);
		virtual unsigned long __stdcall Release(void);

		XProvider& operator=(XProvider const&);

		static long __stdcall Create(Element*, IXProviderCP*, XProvider**);

		long CreateParser(DUIXmlParser**);
		long Initialize(Element*, IXProviderCP*);

		virtual long __stdcall SetButtonClassAcceptsEnterKey(bool);
		virtual long __stdcall CreateXBaby(DirectUI::IXElementCP*, HWND, Element*, unsigned long*, IXBaby**);
		virtual ~XProvider(void);

	protected:
		Element* GetRoot(void);
		void SetHandleEnterKey(bool);

		Element* _RootElement;
		ULONG refCount;
		Element* _RandomElement;
		IXProviderCP* XProviderCP;
	};

}