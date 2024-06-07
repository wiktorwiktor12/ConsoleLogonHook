#pragma once

namespace DirectUI
{
	class UILIB_API SelectionItemProvider
		: public PatternProvider<SelectionItemProvider, ISelectionItemProvider, 7>
		, public ISelectionItemProvider
	{
	public:
		SelectionItemProvider(void);
		virtual ~SelectionItemProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual long __stdcall AddToSelection(void);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall RemoveFromSelection(void);
		virtual long __stdcall Select(void);
		virtual long __stdcall get_IsSelected(int *);
		virtual long __stdcall get_SelectionContainer(IRawElementProviderSimple * *);

	};

	class UILIB_API SelectionItemProxy : IProxy
	{
	protected:
		SelectionItemProxy(void);
	public:
		SelectionItemProxy(SelectionItemProxy const &);
		SelectionItemProxy & operator=(SelectionItemProxy const &);

		static SelectionItemProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);
		
		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);

	};
}