#pragma once

namespace DirectUI
{
	class UILIB_API TableProvider
		: public PatternProvider<TableProvider, ITableProvider, 9>
		, public ITableProvider
	{
	public:
		TableProvider(void);
		virtual ~TableProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual long __stdcall GetColumnHeaders(SAFEARRAY * *);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall GetRowHeaders(SAFEARRAY * *);
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall get_RowOrColumnMajor(RowOrColumnMajor *);
	};

	class UILIB_API TableProxy : public IProxy
	{
	public:
		TableProxy(TableProxy const &);
		TableProxy(void);
		TableProxy & operator=(TableProxy const &);

		static TableProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);

		virtual long DoMethod(int, char *);

	protected:
		virtual void Init(Element *);

	};
}