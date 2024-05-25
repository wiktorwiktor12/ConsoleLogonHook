#pragma once
#include "ui_main.h"
#include <string>

class uiStatusView : public uiWindow
{
public:

    void* statusInstance;
    std::wstring statusText;

    uiStatusView()
    {
        windowTypeId = 4;
    }

    void Draw() override;

    static void InitHooks(uintptr_t baseaddress);
};