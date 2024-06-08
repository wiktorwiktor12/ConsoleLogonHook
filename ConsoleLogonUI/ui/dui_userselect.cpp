#include "dui_userselect.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include <sddl.h>
#include <vector>
#include <atlbase.h>
#include "util/interop.h"

std::vector<SelectableUserOrCredentialControlWrapper> buttons;

void external::UserSelect_SetActive()
{
    //MessageBoxW(0, L"1", L"1", 0);
    HideConsoleUI();
    //auto userSelect = duiManager::Get()->GetWindowOfTypeId<uiUserSelect>(5);
    //if (userSelect)
    //{
    //    SPDLOG_INFO("Setting active status userSelect");
    //    userSelect->SetActive();
    //
    //
    //}
}

void external::SelectableUserOrCredentialControl_Sort()
{
    std::sort(buttons.begin(), buttons.end(), [](SelectableUserOrCredentialControlWrapper& a, SelectableUserOrCredentialControlWrapper& b) { return a.GetText() < b.GetText(); });
}

//TODO: move the texture loading to another thread so that it does not delay the whole program loading the textures
void external::SelectableUserOrCredentialControl_Create(void* actualInstance, const wchar_t* path)
{
    SelectableUserOrCredentialControlWrapper wrapper;
    wrapper.actualInstance = actualInstance;
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

}

void duiUserSelect::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiUserSelect::Begin()
{

}