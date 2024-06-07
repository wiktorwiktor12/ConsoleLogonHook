#pragma once

namespace DirectUI
{
	//此类虚函数全部来自父
	class UILIB_API CCBaseScrollBar
		: public CCBase
		, public BaseScrollBar
	{
	public:
		CCBaseScrollBar(const CCBaseScrollBar &);
		CCBaseScrollBar(unsigned long v1=0);
		CCBaseScrollBar & operator=(const CCBaseScrollBar &);

		virtual ~CCBaseScrollBar(void);

		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static const PropertyInfo* __stdcall LineProp(void);
		static const PropertyInfo* __stdcall MaximumProp(void);
		static const PropertyInfo* __stdcall MinimumProp(void);
		static const PropertyInfo* __stdcall PageProp(void);
		static const PropertyInfo* __stdcall PositionProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static const PropertyInfo* __stdcall TrackingProp(void);

		bool GetTracking(void);
		long Initialize(unsigned int, Element *, unsigned long *);
		long SetTracking(bool);
		void SyncScrollBar(void);


		virtual HWND CreateHWND(HWND);
		virtual IClassInfo * GetClassInfoW(void);
		virtual Element * GetElement(void);
		virtual int GetLine(void);
		virtual int GetMaximum(void);
		virtual int GetMinimum(void);
		virtual int GetPage(void);
		virtual int GetPosition(void);
		virtual bool GetProportional(void);
		virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);
		virtual bool OnPropertyChanging(const PropertyInfo*, int, Value *, Value *);
		virtual long SetLine(int);
		virtual long SetMaximum(int);
		virtual long SetMinimum(int);
		virtual long SetPage(int);
		virtual long SetPosition(int);

	private:
		static IClassInfo * s_pClassInfo;
	};
}