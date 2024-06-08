#pragma once
#include "dui_manager.h"
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

class duiSecurityControl : public duiBaseElement
{
public:

    duiSecurityControl();
    virtual ~duiSecurityControl() override;

    DEFINE_DUIELEMENTCLASS(L"duiSecurityControl");

    std::vector<std::function<void()>> wasInSecurityControlNotifies;

};