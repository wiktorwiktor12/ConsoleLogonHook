#pragma once

namespace DirectUI
{

	class UILIB_API ExpandCollapseProvider
		: public PatternProvider<ExpandCollapseProvider, IExpandCollapseProvider,1>
		, public IExpandCollapseProvider
	{
	public:
		ExpandCollapseProvider(void);

		virtual ~ExpandCollapseProvider(void);;


		virtual ProviderProxyCall GetProxyCreator(void);


		//IUnknown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef(void);

		virtual ULONG STDMETHODCALLTYPE Release(void);


		//IExpandCollapseProvider
		virtual HRESULT STDMETHODCALLTYPE Expand(void);

		virtual HRESULT STDMETHODCALLTYPE Collapse(void);

		virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ExpandCollapseState(
			/* [retval][out] */ __RPC__out enum ExpandCollapseState *pRetVal);

	};

	class UILIB_API ExpandCollapseProxy : public IProxy
	{
	public:
		ExpandCollapseProxy(ExpandCollapseProxy const &);
		ExpandCollapseProxy(void);
		ExpandCollapseProxy & operator=(ExpandCollapseProxy const &);

		static ExpandCollapseProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);
		
		//1
		virtual long DoMethod(int, char *);


	protected:
		//2
		virtual void Init(Element *);
	};

}