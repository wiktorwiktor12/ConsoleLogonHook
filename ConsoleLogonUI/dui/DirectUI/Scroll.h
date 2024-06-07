#pragma once

namespace DirectUI
{
	class UILIB_API ScrollProvider
		: public PatternProvider<ScrollProvider, IScrollProvider, 5>
		, public IScrollProvider
	{
	public:
		ScrollProvider(void);
		virtual ~ScrollProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall QueryInterface(struct _GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall Scroll(enum ScrollAmount, enum ScrollAmount);
		virtual long __stdcall SetScrollPercent(double, double);
		virtual long __stdcall get_HorizontalScrollPercent(double *);
		virtual long __stdcall get_HorizontalViewSize(double *);
		virtual long __stdcall get_HorizontallyScrollable(int *);
		virtual long __stdcall get_VerticalScrollPercent(double *);
		virtual long __stdcall get_VerticalViewSize(double *);
		virtual long __stdcall get_VerticallyScrollable(int *);

	};

	class UILIB_API ScrollProxy : public IProxy
	{
	public:
		ScrollProxy(ScrollProxy const &);
		ScrollProxy(void);
		ScrollProxy & operator=(ScrollProxy const &);

		static ScrollProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);

		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);
	private:
		BaseScrollBar * GetScrollBar(bool);
		long GetScrollPercent(bool, double *);
		long GetScrollable(bool, int *);
		long GetViewSize(bool, double *);
		long GetViewer(class Viewer * *);
		long Scroll(enum ScrollAmount, enum ScrollAmount);
		long ScrollLine(bool, bool);
		long ScrollPage(bool, bool);
		long ScrollToHorizontalPosition(int, bool);
		long ScrollToVerticalPosition(int, bool);
		long SetPercent(BaseScrollBar *, double);
		long SetScrollPercent(double, double);

	};
}