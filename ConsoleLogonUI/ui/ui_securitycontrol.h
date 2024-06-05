#pragma once
#include "ui_main.h"
#include <functional>
#include <string>

struct SecurityOptionControlWrapper
{
    void* actualInstance;

    SecurityOptionControlWrapper(void* instance)
    {
        actualInstance = instance;
    }

    std::wstring getString();

    void Press();
};

class uiSecurityControl : public uiWindow
{
public:

    std::vector<std::function<void()>> wasInSecurityControlNotifies;

    uiSecurityControl()
    {
        windowTypeId = 2;
    }

    void Draw() override;
};