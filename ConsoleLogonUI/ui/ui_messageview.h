#pragma once
#include "ui_main.h"
#include <string>

struct MessageOptionControlWrapper
{
    void* actualInstance;
    int optionflag;

    void Press();

    std::wstring GetText();
};

class uiMessageView : public uiWindow
{
public:

    std::wstring message;

    uiMessageView()
    {
        windowTypeId = 3;
    }

    void Draw() override;
};