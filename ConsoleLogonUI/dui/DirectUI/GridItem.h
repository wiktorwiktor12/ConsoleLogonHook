#pragma once

namespace DirectUI
{
	class UILIB_API GridItemProvider
		: PatternProvider<GridItemProvider, IGridItemProvider, 2>
		, public IGridItemProvider
	{
	public:
		GridItemProvider(void);
		virtual ~GridItemProvider(void);

		virtual unsigned long __stdcall AddRef(void);
		virtual ProviderProxyCall GetProxyCreator(void);;
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall get_Column(int *);
		virtual long __stdcall get_ColumnSpan(int *);
		virtual long __stdcall get_ContainingGrid(IRawElementProviderSimple * *);
		virtual long __stdcall get_Row(int *);
		virtual long __stdcall get_RowSpan(int *);
	};

	class UILIB_API GridItemProxy : public IProxy
	{
	public:
		GridItemProxy(GridItemProxy const &);
		GridItemProxy(void);
		GridItemProxy & operator=(GridItemProxy const &);

		static GridItemProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);
		//1
		virtual long DoMethod(int, char *);
	protected:
		//2
		virtual void Init(Element *);

	private:
		long GetColumn(int *);
		long GetContainingGrid(IRawElementProviderSimple * *);
		long GetRow(int *);
	};
}