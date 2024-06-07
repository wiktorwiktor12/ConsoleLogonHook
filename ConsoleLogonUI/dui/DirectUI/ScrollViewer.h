#pragma once

namespace DirectUI
{
	class UILIB_API ScrollViewer
		: public BaseScrollViewer
	{
	public:
		ScrollViewer(ScrollViewer const &);
		ScrollViewer(void);
		virtual ~ScrollViewer(void);
		ScrollViewer & operator=(ScrollViewer const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnListenedPropertyChanged(Element *, PropertyInfo const *, int, Value *, Value *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
	protected:
		//1
		virtual long CreateScrollBars(void);
		//2
		virtual long AddChildren(void);
		//3
		virtual BaseScrollBar * GetHScroll(void);
		//4
		virtual BaseScrollBar * GetVScroll(void);
	
	private:
		static IClassInfo * s_pClassInfo;

	};
}