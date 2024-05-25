#include "ui_messageview.h"
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>

__int64(__fastcall* MessageView__RuntimeClassInitialize)(__int64 a1, HSTRING a2, HSTRING a3, char a4, __int64 a5, __int64 a6);
__int64 MessageView__RuntimeClassInitialize_Hook(__int64 a1, HSTRING a2, HSTRING a3, char a4, __int64 a5, __int64 a6)
{
    auto convertString = [&](HSTRING str) -> std::wstring
        {
            const wchar_t* convertedString = fWindowsGetStringRawBuffer(a2, 0);
            return convertedString;
        };
   
    std::wstring string = convertString(a3);
    auto res = MessageView__RuntimeClassInitialize(a1, a2, a3, a4, a5, a6);
    if (string.size() == 0)
        string = convertString(a3);

    //SPDLOG_INFO("MessageView__RuntimeClassInitialize a1[{}] a2[{}] a3[{}] a4[{}] a5[{}] a6[{}]", (void*)a1, ws2s(convertString(a2)).c_str(), ws2s(string).c_str(), (int)a4, (void*)a5, (void*)a6);
    SPDLOG_INFO("a3 {} length {}", ws2s(string), string.size());
    return res;
}


void uiMessageView::Draw()
{

}

void uiMessageView::InitHooks(uintptr_t baseaddress)
{
    MessageView__RuntimeClassInitialize = decltype(MessageView__RuntimeClassInitialize)(baseaddress + 0x389B0);

    Hook(MessageView__RuntimeClassInitialize, MessageView__RuntimeClassInitialize_Hook);
}
