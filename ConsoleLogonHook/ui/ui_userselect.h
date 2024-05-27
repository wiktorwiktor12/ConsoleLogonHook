#pragma once
#include "ui_main.h"
#include <string>

struct SelectableUserOrCredentialControlWrapper
{
public:
    void* actualInstance;
    std::wstring text;
    WORD virtualKeyCodeToPress = VK_RETURN;
    int controlHandleIndex = -1;
    unsigned long long tickMarkedPressed = 0;
    bool markedPressed = false;
    bool hastext = false;

    std::wstring GetText();

    void Press();
    bool isCredentialControl();
};

class uiUserSelect : public uiWindow
{
public:

    uiUserSelect()
    {
        windowTypeId = 5;
    }

    void Tick() override;
    void Draw() override;

    static void InitHooks(uintptr_t baseaddress);
};