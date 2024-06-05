#pragma once
#include <functional>

//struct SecurityOptionControlWrapper
//{
//    void* actualInstance;
//
//    SecurityOptionControlWrapper(void* instance)
//    {
//        actualInstance = instance;
//    }
//
//    wchar_t* getString()
//    {
//        return *(wchar_t**)(__int64(actualInstance) + 0x48);
//    }
//
//    void Press();
//};

class uiSecurityControl
{
public:

    std::vector<std::function<void()>> wasInSecurityControlNotifies;

    static void InitHooks(uintptr_t baseaddress);
};