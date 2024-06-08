#pragma once
#include "dui_manager.h"
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

class duiUserSelect : public duiBaseElement
{
public:

    duiUserSelect();
    virtual ~duiUserSelect() override;

    DEFINE_DUIELEMENTCLASS(L"duiUserSelect");

	bool wasInSelectedCredentialView = false;


};