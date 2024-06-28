#pragma once
#include <Windows.h>
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

inline HANDLE uiUserSelectThreadHandle;

static class uiUserSelect
{
public:

    

    static void InitHooks(uintptr_t baseaddress);
};