#pragma once
#include "ui_main.h"

class uiMessageView : public uiWindow
{
public:

    uiMessageView()
    {
        windowTypeId = 3;
    }

    void Draw() override;

    static void InitHooks(uintptr_t baseaddress);
};