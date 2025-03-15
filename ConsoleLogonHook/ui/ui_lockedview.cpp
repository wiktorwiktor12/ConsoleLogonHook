#include "ui_lockedview.h"
#include "detours.h"

// LockedView::RuntimeClassInitialize is often inlined. Rely on BasicTextControl's
// initializer instead.
HRESULT (*BasicTextControl_RuntimeClassInitialize)(void *, void *, UINT);
HRESULT BasicTextControl_RuntimeClassInitialize_hook(void *pThis, void *view, UINT stringResourceId)
{
	if (stringResourceId == 100)
	{
		external::LockedView_SetActive();
	}
	return BasicTextControl_RuntimeClassInitialize(pThis, view, stringResourceId);
}

void uiLockedView::InitHooks(uintptr_t baseaddress)
{
	BasicTextControl_RuntimeClassInitialize = memory::FindPatternCached<decltype(BasicTextControl_RuntimeClassInitialize)>(
		"BasicTextControl_RuntimeClassInitialize",
		{ "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ?? 48 8B F2 48 8B F9 48 83 C1" }
	);

	Hook(BasicTextControl_RuntimeClassInitialize, BasicTextControl_RuntimeClassInitialize_hook);
}