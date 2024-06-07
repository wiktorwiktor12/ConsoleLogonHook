#pragma once

namespace DirectUI
{
	//此类所有虚函数继承自父类，无需额外调整
	class UILIB_API AnimationStrip :public Element
	{
	public:
		AnimationStrip(const AnimationStrip&);
		AnimationStrip();
		AnimationStrip& operator=(const AnimationStrip&);
		virtual ~AnimationStrip();

		static long __stdcall Create(Element*, unsigned long*, Element**);
		static const PropertyInfo* __stdcall FrameDurationProp();
		static const PropertyInfo* __stdcall FrameIndexProp();
		static const PropertyInfo* __stdcall FrameWidthProp();
		static IClassInfo* __stdcall GetClassInfoPtr();
		static const PropertyInfo* __stdcall PlayProp(void);
		static long __stdcall Register();
		static void __stdcall SetClassInfoPtr(IClassInfo*);

		int GetFrameDuration();
		int GetFrameIndex();
		int GetFrameWidth();
		bool GetPlay();
		long Initialize(unsigned int, Element*, unsigned long *);
		long SetFrameDuration(int);
		long SetFrameIndex(int);
		long SetFrameWidth(int);
		long SetPlay(bool);
		virtual IClassInfo* GetClassInfoW();

	protected:
		virtual void OnDestroy(void);
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value *, Value *);
		virtual void Paint(HDC, const RECT*, const RECT*, RECT *, RECT *);

		void AdvanceFrame(void);
		long LoadImagesIntoAnimationStrip();
		void OnAction(GMA_ACTIONINFO *);

		static void __stdcall RawActionProc(GMA_ACTIONINFO *);

	private:
		long LoadCommonControlExports();
		long Start();
		void Stop();
		void UnloadCommonControlExports();
		static IClassInfo* s_pClassInfo;
	};
}