#pragma once
#include "dui_manager.h"
#include <string>

struct EditControlWrapper
{
    void* actualInstance;
    std::string inputBuffer = ""; // for imgui
    std::string fieldNameCache = "";

    std::wstring GetFieldName();
    std::wstring GetInputtedText();
    void SetInputtedText(std::wstring input);
    bool isVisible();
};

class duiSelectedCredentialView : public duiBaseElement
{
public:
    duiSelectedCredentialView();
    virtual ~duiSelectedCredentialView() override;

    DEFINE_DUIELEMENTCLASS(L"duiSelectedCredentialView");

    //TODO: LOAD THESE FROM A MUI, SO WE HAVE TRANSLATIONS
    std::wstring switchUser = L"Switch User";
    std::wstring cancel = L"Cancel";
    std::wstring accountNameToDisplay;
    ID3D11ShaderResourceView* texture = 0;
    int flag = 0;

    bool textureExists = true;
    bool hasSetupNotify = false;

};