#pragma once


namespace DirectUI
{
	class UILIB_API PushButton : public AutoButton
	{
	public:
		PushButton(PushButton const &);
		PushButton(void);
		virtual ~PushButton(void);
		PushButton & operator=(PushButton const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static PropertyInfo const * __stdcall EnforceSizeProp(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static UID __stdcall Hosted(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		
		bool GetEnforceSize(void);
		long SetEnforceSize(bool);

		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual void OnHosted(Element *);
		virtual void OnUnHosted(Element *);

	private:
		static IClassInfo * s_pClassInfo;
		void FireHostEvent(Element *, bool);

	};
}