#pragma once
#include "dui_manager.h"
#include <string>

class duiStatusView : public duiBaseElement
{
public:

    duiStatusView();
    virtual ~duiStatusView() override;

    DEFINE_DUIELEMENTCLASS(L"duiStatusView");

    std::wstring statusText;

};