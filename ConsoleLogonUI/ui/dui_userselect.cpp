#include "dui_userselect.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include <sddl.h>
#include <vector>
#include <atlbase.h>
#include "util/interop.h"
#include "resources/resource.h"

std::vector<SelectableUserOrCredentialControlWrapper> buttons;

void external::UserSelect_SetActive()
{
    //MessageBoxW(0, L"1", L"1", 0);
    HideConsoleUI();
    duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_USERSELECT), [](DirectUI::Element* elm) -> void
        {
            return;
        });
    //auto userSelect = duiManager::Get()->GetWindowOfTypeId<uiUserSelect>(5);
    //if (userSelect)
    //{
    //    SPDLOG_INFO("Setting active status userSelect");
    //    userSelect->SetActive();
    //
    //
    //}
}

int sizeArray[] = { 1,1,2,3,4,5,3,4,4,5,5,4,4,5,5 };

void external::SelectableUserOrCredentialControl_Sort()
{
    std::sort(buttons.begin(), buttons.end(), [](SelectableUserOrCredentialControlWrapper& a, SelectableUserOrCredentialControlWrapper& b) { return a.GetText() < b.GetText(); });

    duiManager::SendWorkToUIThread([](void* param) -> void
        {
            auto& userButtons = *reinterpret_cast<std::vector<SelectableUserOrCredentialControlWrapper>*>(param);
            auto pDuiManager = duiManager::Get();
            auto UserList = (DirectUI::Selector*)pDuiManager->pUIElement->FindDescendent(ATOMID(L"UserList"));
            auto InsideFrame = pDuiManager->pUIElement->FindDescendent(ATOMID(L"InsideFrame"));
            DirectUI::Value* v13;
            auto extent = InsideFrame->GetExtent(&v13);
            int xExtent = extent->cx;
            v13->Release();

            int DPI = 96;

            int width = 0;
            int userCount = userButtons.size() & 0xFFFFFFF;
            if (userCount >= 0x10)
                width = 5;
            else
                width = sizeArray[std::clamp(userCount,0,(int)(_ARRAYSIZE(sizeArray) - 1))];

            int v10 = 180 * width + 12;
            while (xExtent < MulDiv(v10, DPI, 96))
            {
                --width;
                v10 -= 180;
                xExtent = InsideFrame->GetExtent(&v13)->cx;
                v13->Release();
                DPI = 96;
            }
            int v11 = 96;
            int v12 = MulDiv(180 * width + 12, v11, 96);
            UserList->SetWidth(v12);

            UserList->SetVisible(true);
            for (int i = 0; i < userButtons.size(); ++i)
            {
                auto& wrapper = userButtons[i];
                DirectUI::Button* btn = 0;
                HRESULT hr = pDuiManager->pParser->CreateElement(
                    (DirectUI::UCString)L"UserTileTemplate",
                    NULL,
                    NULL,
                    NULL,
                    (DirectUI::Element**)&btn
                );

                if (!btn)
                {
                    MessageBox(0, L"CreateElement failed", 0, 0);
                    continue;
                }

                int DPI = 96;
                int width = MulDiv(180, DPI, 96);
                btn->SetWidth(width);

                std::wstring username = wrapper.GetText();

                auto userText = btn->FindDescendent(ATOMID(L"UserText"));
                if (userText)
                    userText->SetContentString((DirectUI::UCString)username.c_str());

                auto pic = btn->FindDescendent(ATOMID(L"Picture"));
                if (pic)
                {
                    auto graphic = DirectUI::Value::CreateGraphic(wrapper.pfp, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
                    if (!graphic) return;

                    pic->SetValue(DirectUI::Element::ContentProp, 1, graphic);
                    graphic->Release();
                }

                DirectUIElementAdd(UserList, btn);

                btn->SetVisible(true);
            }
            DumpDuiTree(UserList, 0);
        }, &buttons);
}

//TODO: move the texture loading to another thread so that it does not delay the whole program loading the textures
void external::SelectableUserOrCredentialControl_Create(void* actualInstance, const wchar_t* path)
{
    SelectableUserOrCredentialControlWrapper wrapper;
    wrapper.actualInstance = actualInstance;
    wrapper.pfp = GetHBITMAPFromImageFile(const_cast<WCHAR*>(path));
    //int w = 0;
    //int h = 0;
    //if (duiManager::LoadTextureFromFile(ws2s(path).c_str(), &wrapper.texture, &w, &h))
    //    SPDLOG_INFO("TEXTURE LOADED");

    buttons.push_back(wrapper);
}

void external::SelectableUserOrCredentialControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("Found button instance and removing!");
            buttons.erase(buttons.begin() + i);
            break;
        }
    }
}

std::wstring SelectableUserOrCredentialControlWrapper::GetText()
{
    if (hastext)
        return text;

    text = external::SelectableUserOrCredentialControl_GetText(actualInstance);
    hastext = true;
    return text;
}

void SelectableUserOrCredentialControlWrapper::Press()
{
    return external::SelectableUserOrCredentialControl_Press(actualInstance);
}

bool SelectableUserOrCredentialControlWrapper::isCredentialControl()
{
    return external::SelectableUserOrCredentialControl_isCredentialControl(actualInstance);
}

DirectUI::IClassInfo* duiUserSelect::Class = NULL;

duiUserSelect::duiUserSelect()
{
}

duiUserSelect::~duiUserSelect()
{
}

HRESULT duiUserSelect::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiUserSelect* instance = (duiUserSelect*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiUserSelect));

    if (instance != NULL)
    {
        new (instance) duiUserSelect();
        hr = instance->Initialize(0, rootElement, debugVariable);
        if (SUCCEEDED(hr))
        {
            *newElement = instance;
        }
        else
        {
            if (instance != NULL)
            {
                instance->Destroy(TRUE);
                instance = NULL;
            }
        }
    }

    return hr;
}

DirectUI::IClassInfo* duiUserSelect::GetClassInfoW()
{
    return duiUserSelect::Class;
}

void duiUserSelect::OnEvent(DirectUI::Event* iev)
{
    if (iev->flag != DirectUI::GMF_BUBBLED)
        return;
    if (!iev->handled)
        DirectUI::Element::OnEvent(iev);

    if (!iev->target || !iev->target->GetParent()) return;

    if (iev->target->GetParent()->GetID() == ATOMID(L"UserList") && iev->type == DirectUI::Button::Click)
    {
        auto tile = iev->target;
        auto usernameElement = tile->FindDescendent(ATOMID(L"UserText"));
        if (usernameElement)
        {
            DirectUI::Value* val;
            std::wstring username = (const wchar_t*)usernameElement->GetContentString(&val);
            val->Release();
            
            for (int i = 0; i < buttons.size(); ++i)
            {
                auto& wrapper = buttons[i];
                if (wrapper.GetText() == username)
                {
                    wrapper.Press();
                    break;
                }
            }
        }
    }
}

void duiUserSelect::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiUserSelect::Begin()
{

}