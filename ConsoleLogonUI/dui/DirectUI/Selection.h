#pragma once

namespace DirectUI
{
	class UILIB_API SelectionProvider
		: public PatternProvider<SelectionProvider, ISelectionProvider, 8>
		, public ISelectionProvider
	{
	public:
		SelectionProvider(void);
		virtual ~SelectionProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall GetSelection(SAFEARRAY * *);
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall get_CanSelectMultiple(int *);
		virtual long __stdcall get_IsSelectionRequired(int *);

	};

	class UILIB_API SelectionProxy : public IProxy
	{
	protected:
		SelectionProxy(void);
	public:
		SelectionProxy(SelectionProxy const &);
		SelectionProxy & operator=(SelectionProxy const &);

		static SelectionProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);
		
		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);

	};
}