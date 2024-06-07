#pragma once

namespace DirectUI
{
	class UILIB_API StyledScrollViewer
		: public BaseScrollViewer
	{
	public:
		StyledScrollViewer(StyledScrollViewer const &);
		StyledScrollViewer(void);
		virtual ~StyledScrollViewer(void);
		StyledScrollViewer & operator=(StyledScrollViewer const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnListenedPropertyChanged(Element *, PropertyInfo const *, int, Value *, Value *);
	protected:
		virtual long AddChildren(void);
		virtual long CreateScrollBars(void);
		virtual BaseScrollBar * GetHScroll(void);
		virtual BaseScrollBar * GetVScroll(void);
	private:
		static IClassInfo * s_pClassInfo;

	};
}