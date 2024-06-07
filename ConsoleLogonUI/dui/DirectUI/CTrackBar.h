#pragma once

namespace DirectUI
{
	class UILIB_API CCTrackBar : public CCBase
	{
	public:
		CCTrackBar(CCTrackBar const &);
		CCTrackBar(void);
		virtual ~CCTrackBar(void);
		CCTrackBar & operator=(CCTrackBar const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static const PropertyInfo* __stdcall LineSizeProp(void);
		static const PropertyInfo* __stdcall ThumbPositionProp(void);
		static const PropertyInfo* __stdcall RangeMaxProp(void);
		static const PropertyInfo* __stdcall RangeMinProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		int GetLineSize(void);
		int GetRangeMax(void);
		int GetRangeMin(void);
		int GetThumbPosition(void);
		long SetLineSize(int);
		long SetRangeMax(int);
		long SetRangeMin(int);
		long SetThumbPosition(int);
		
		virtual IClassInfo * GetClassInfoW(void);
		virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);
		virtual bool OnPropertyChanging(const PropertyInfo*, int, Value *, Value *);

	protected:
		virtual void PostCreate(HWND);
	private:
		static IClassInfo * s_pClassInfo;

	};
}