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

    bool hasSetupNotify = false;

    std::wstring accountNameToDisplay;

    uiSelectedCredentialView()
    {
        windowTypeId = 6;
    }

    void Begin() override;
    void Tick() override;
    void Draw() override;

    static void InitHooks(uintptr_t baseaddress);
};