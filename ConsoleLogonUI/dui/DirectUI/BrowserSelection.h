#pragma once

namespace DirectUI
{
	class UILIB_API BrowserSelectionProxy : public IProxy
	{
	public:
		BrowserSelectionProxy(BrowserSelectionProxy const &);
		BrowserSelectionProxy(void);
		BrowserSelectionProxy & operator=(BrowserSelectionProxy const &);
		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);
	private:
		long GetIsSelectionRequired(int *);
		long GetSelection(SAFEARRAY * *);

	};
}