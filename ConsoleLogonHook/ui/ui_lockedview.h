#pragma once
#include <windows.h>
#include <util/interop.h>
#include <util/memory_man.h>

class uiLockedView
{
public:
	static void InitHooks(uintptr_t baseaddress);
};