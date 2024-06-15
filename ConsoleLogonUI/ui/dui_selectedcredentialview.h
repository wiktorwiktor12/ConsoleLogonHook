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
    virtual void OnInput(DirectUI::InputEvent* a2);

    //TODO: LOAD THESE FROM A MUI, SO WE HAVE TRANSLATIONS
    std::wstring switchUser = L"Switch User";
    std::wstring cancel = L"Cancel";
    std::wstring accountNameToDisplay;
    int flag = 0;

    static DirectUI::Element* CreateStringField(DirectUI::Element* UserTile, std::wstring content, bool bVisible, bool bIsSmall = false);
    static DirectUI::Element* CreateEditField(DirectUI::Element* UserTile, std::wstring content, bool bVisible, bool bShowSubmit = false, bool bIsPassword = false);
    static DirectUI::Element* CreateCommandLinkField(DirectUI::Element* UserTile, std::wstring content, bool bVisible);

    bool textureExists = true;
    bool hasSetupNotify = false;

};