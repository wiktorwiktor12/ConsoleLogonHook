#pragma once
#include "ui_main.h"
#include <string>

class uiStatusView : public uiWindow
{
public:

    std::wstring statusText;

    uiStatusView()
    {
        windowTypeId = 4;
    }

    void Draw() override;
};