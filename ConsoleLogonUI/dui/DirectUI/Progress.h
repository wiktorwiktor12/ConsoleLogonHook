#pragma once

namespace DirectUI
{
	class UILIB_API Progress : public Element
	{
	public:
		Progress(Progress const &);
		Progress(void);
		virtual ~Progress(void);
		Progress & operator=(Progress const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static PropertyInfo const * __stdcall PositionProp(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static PropertyInfo const * __stdcall MaximumProp(void);
		static PropertyInfo const * __stdcall MinimumProp(void);

		int GetMaximum(void);
		int GetMinimum(void);
		int GetPosition(void);
		long Initialize(Element *, unsigned long *);
		long SetMaximum(int);
		long SetMinimum(int);
		long SetPosition(int);

		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual void Paint(HDC, RECT const *, RECT const *, RECT *, RECT *);
	private:
		static IClassInfo * s_pClassInfo;

	};

	class UILIB_API ProgressRangeValueProxy : public IProxy
	{
	public:
		ProgressRangeValueProxy(ProgressRangeValueProxy const &);
		ProgressRangeValueProxy(void);
		ProgressRangeValueProxy & operator=(ProgressRangeValueProxy const &);

		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);
	};
}