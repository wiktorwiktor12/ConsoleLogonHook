#pragma once
#include "ui_main.h"
#include <string>

struct SelectableUserOrCredentialControlWrapper
{
public:
    void* actualInstance;
    std::wstring text;
    ID3D11ShaderResourceView* texture = 0;
    bool hastext = false;

    std::wstring GetText();

    void Press();
    bool isCredentialControl();
};

class uiUserSelect : public uiWindow
{
public:

	bool wasInSelectedCredentialView = false;

    uiUserSelect()
    {
        windowTypeId = 5;
    }

    void Tick() override;
    void Draw() override;
};