#pragma once

namespace DirectUI
{
	class UILIB_API TableItemProvider
		: public PatternProvider<TableItemProvider, ITableItemProvider, 10>
		, public ITableItemProvider
	{
	public:
		TableItemProvider(void);
		virtual ~TableItemProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual long __stdcall GetColumnHeaderItems(SAFEARRAY * *);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall GetRowHeaderItems(SAFEARRAY * *);
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);

	};

	class UILIB_API TableItemProxy : public IProxy
	{
	public:
		TableItemProxy(TableItemProxy const &);
		TableItemProxy(void);
		TableItemProxy & operator=(TableItemProxy const &);

		static TableItemProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);
		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);

	};
}