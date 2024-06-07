#pragma once

namespace DirectUI
{
	class UILIB_API SelectorSelectionItemProxy : public IProxy
	{
	public:
		SelectorSelectionItemProxy(SelectorSelectionItemProxy const &);
		SelectorSelectionItemProxy(void);
		SelectorSelectionItemProxy & operator=(SelectorSelectionItemProxy const &);

		virtual long DoMethod(int, char *);
	protected:
		virtual void Init(Element *);
	private:
		long AddToSelection(void);
		long GetSelectionContainer(IRawElementProviderSimple * *);
		long Select(void);

	};
}