#pragma once
#include "ui_main.h"
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

class uiSelectedCredentialView : public uiWindow
{
public:

    //TODO: LOAD THESE FROM A MUI, SO WE HAVE TRANSLATIONS
    std::string switchUser = "Switch User";
    std::string cancel = "Cancel";
    std::wstring accountNameToDisplay;
    ID3D11ShaderResourceView* texture = 0;

    bool textureExists = true;
    bool hasSetupNotify = false;

    uiSelectedCredentialView()
    {
        windowTypeId = 6;
    }

    void Begin() override;
    void Tick() override;
    void Draw() override;
};