#pragma once

namespace DirectUI
{
	//问号部分无法确定顺序
	class UILIB_API TaskPage
		: public IElementListener
		, public IXProviderCP
	{
	public:
		TaskPage(TaskPage const &);
		TaskPage();
		virtual ~TaskPage(void);
		TaskPage & operator=(TaskPage const &);
		operator struct _PSP *(void);

		long DUICreatePropertySheetPage(HINSTANCE);
		HWND GetParentHWND(void);
	
	protected:
		Element * GetElement(void);
		LONG_PTR PropSheet_SendMessage(UINT message, WPARAM, LPARAM);
		
		//IElementListener
		virtual void OnListenerAttach(Element *);
		virtual void OnListenerDetach(Element *);
		virtual bool OnListenedPropertyChanging(Element *, PropertyInfo const *, int, Value *, Value *);
		virtual void OnListenedPropertyChanged(Element *, PropertyInfo const *, int, Value *, Value *);
		virtual void OnListenedEvent(Element *, Event *);
		virtual void OnListenedInput(Element *, InputEvent *);

		//1
		virtual long LoadParser(DUIXmlParser * *);
		//2
		virtual long LoadPage(HWNDElement *, HINSTANCE, Element * *, DUIXmlParser * *);
		//??
		virtual unsigned int GetPageRCID(void);
		virtual unsigned short const * GetPageResID(void);
		//5
		virtual void InitPropSheetPage(PROPSHEETPAGEW *);

		//??
		virtual LRESULT OnKillActive(void);
		virtual LRESULT OnQueryCancel(void);
		virtual Element * OnQueryInitialFocus(void);
		virtual LRESULT OnReset(void);
		virtual LRESULT OnSetActive(void);
		virtual LRESULT OnWizBack(void);
		virtual LRESULT OnWizFinish(void);
		virtual LRESULT OnWizNext(void);
		//14
		virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

	private:
		static LRESULT __stdcall StaticXHostSubclassProc(HWND, UINT message, WPARAM wParam, LPARAM lParam);
		static void __stdcall StaticXmlParserError(unsigned short const *, unsigned short const *, int, void *);

		void FreeComCtl32(void);
		int OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		long LoadComCtl32(void);
		long LoadPage(Element * *, Element *, DUIXmlParser * *);

		//IXProviderCP
		virtual long CreateDUICP(HWNDElement *, HWND, HWND, Element * *, DUIXmlParser * *);
		virtual long CreateParserCP(DUIXmlParser * *);
		virtual void DestroyCP(void);

	};
}