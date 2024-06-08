#pragma once
#include "dui_manager.h"
#include <string>

struct MessageOptionControlWrapper
{
    void* actualInstance;
    int optionflag;

    void Press();

    std::wstring GetText();
};

class duiMessageView : public duiBaseElement
{
public:
    duiMessageView();
    virtual ~duiMessageView() override;

    DEFINE_DUIELEMENTCLASS(L"duiMessageView");

    std::wstring message;

};