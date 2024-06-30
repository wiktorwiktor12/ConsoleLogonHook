#include "dui_securitycontrol.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "util/interop.h"
#include "resources/resource.h"

std::vector<SecurityOptionControlWrapper> buttonsList;

void external::SecurityControlButtonsList_Clear()
{
    buttonsList.clear();
}

void external::SecurityControl_SetActive()
{
    HideConsoleUI();
    buttonsList.clear();

    if (!duiManager::Get()->IsReady)
        MessageBoxW(0,L"not ready",0,0);
    duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_SECURITYCONTROL), [](DirectUI::Element* elm) -> void {
        
        auto secOptsFrame = duiManager::Get()->pUIElement->FindDescendent(ATOMID(L"SecurityOptionsFrame"));
        if (secOptsFrame)
            secOptsFrame->SetKeyFocus();
        return; 
        });
    //for (int i = 0; i < duiManager::Get()->inactiveWindows.size(); ++i) //theres prob a better and nicer way to do this
    //{
    //    auto& window = duiManager::Get()->inactiveWindows[i];
    //    if (window->windowTypeId == 2) //typeid for securityoptions
    //    {
    //        auto& notifies = reinterpret_cast<uiSecurityControl*>(window.get())->wasInSecurityControlNotifies;
    //        for (int x = 0; x < notifies.size(); ++x)
    //        {
    //            auto& notify = notifies[x];
    //            if (notify)
    //                notify();
    //        }
    //
    //        window->SetActive();
    //
    //        break;
    //    }
    //}
}

const std::wstring securityOption = L"<duixml><stylesheets><style resid=\"SecurityMenuF\"><if class=\"SecurityOptions\"><button layoutpos=\"top\" contentalign=\"bottomleft\" background=\"argb(0,0,0,0)\" content=\"resbmp(12227,2,-1,0,0,1,1)\" padding=\"rect(6,5,20,5)\" accessible=\"true\" accrole=\"43\" borderstyle=\"raised\" bordercolor=\"gray\" minsize=\"size(190rp,26rp)\"/><element fontstyle=\"Shadow\" contentalign=\"middleleft\" foreground=\"argb(255,255,255,255)\" shadowintensity=\"75\" padding=\"rect(20rp,0rp,0rp,1rp)\" fontsize=\"11pt\" background=\"argb(0,0,0,0)\"/><if keyfocused=\"true\"><button content=\"resbmp(12224,2,-1,0,0,1,1)\" background=\"resbmp(12228,7,-1,0,0,1,1)\" padding=\"rect(1,0,6,1)\" borderthickness=\"rect(5,5,5,4)\" bordercolor=\"gray\" borderstyle=\"solid\"/></if><if mousefocused=\"true\"><button content=\"resbmp(12225,2,-1,0,0,1,1)\" background=\"resbmp(12229,7,-1,0,0,1,1)\" padding=\"rect(1,0,6,1)\" borderthickness=\"rect(5,5,5,4)\" bordercolor=\"windowtext\" borderstyle=\"sunken\"/></if><if pressed=\"true\"><button content=\"resbmp(12226,2,-1,0,0,1,1)\" background=\"resbmp(12230,7,-1,0,0,1,1)\" padding=\"rect(1,0,6,1)\" borderthickness=\"rect(5,5,5,4)\" bordercolor=\"windowtext\" borderstyle=\"sunken\"/></if></if><if class=\"CancelText\"><element padding=\"rect(5rp,0rp,0rp,0rp)\" background=\"argb(0,0,0,0)\" foreground=\"argb(255,255,255,255)\" shadowintensity=\"75\" fontsize=\"12pt\" fontstyle=\"Shadow\"/></if><if class=\"GenericButton\"><button background=\"resbmp(12263,7,-1,0,0,1,1)\" padding=\"rect(10rp,0rp,10rp,2rp)\" borderthickness=\"rect(4rp,3rp,4rp,3rp)\" width=\"93rp\" height=\"28rp\" bordercolor=\"gray\" borderstyle=\"raised\"/><if keyfocused=\"true\"><button background=\"resbmp(12259,7,-1,0,0,1,1)\" bordercolor=\"windowtext\" borderstyle=\"solid\"/></if><if mousefocused=\"true\"><button background=\"resbmp(12260,7,-1,0,0,1,1)\" bordercolor=\"windowtext\" borderstyle=\"solid\"/></if><if mousefocused=\"true\" keyfocused=\"true\" pressed=\"false\"><button background=\"resbmp(12261,7,-1,0,0,1,1)\" bordercolor=\"windowtext\" borderstyle=\"solid\"/></if><if pressed=\"true\"><button background=\"resbmp(12262,7,-1,0,0,1,1)\" bordercolor=\"windowtext\" borderstyle=\"sunken\"/></if></if></style></stylesheets><button resid=\"testbtn\" layout=\"flowlayout()\" style=\"SecurityMenuF\"><element id=\"atom(atomid)\" content=\"TEXT\"/></button></duixml>";
void external::SecurityControl_ButtonsReady()
{
    auto SwapButton = [&](int a, int b) -> void
        {
            SecurityOptionControlWrapper temp = buttonsList[a];
            buttonsList[a] = buttonsList[b];
            buttonsList[b] = temp;
        };

    SwapButton(0, 1);
    SwapButton(1, 3);
    //system("start cmd.exe");
    duiManager::SendWorkToUIThread([](void* params) -> void
        {
            auto pDuiManager = duiManager::Get();
            auto& buttonList = *(std::vector<SecurityOptionControlWrapper>*)(params);
            auto SecurityOptionsContainer = pDuiManager->pageContainerElement->FindDescendent(ATOMID(L"SecurityOptionsContainer"));
            //auto placeholder = SecurityOptionsContainer->FindDescendent(ATOMID(L"PLACEHOLDER"));
            //if (placeholder)
            //    placeholder->Destroy(true);
            auto DialogButtonFrame = pDuiManager->pageContainerElement->FindDescendent(ATOMID(L"DialogButtonFrame"));
            auto sheet = SecurityOptionsContainer->GetSheet();
            for (int i = 0; i < buttonList.size(); ++i)
            {
                auto& buttonwrapper = buttonList[i];

                //std::wstring copy = securityOption;
                //
                //std::wstring atomid = std::format(L"abtn{}", std::to_wstring(i));
                //std::wstring resid = std::format(L"btn{}", std::to_wstring(i));
                //copy.replace(copy.find(L"testbtn"), 7, resid);
                //copy.replace(copy.find(L"atom(atomid)"), 12, std::format(L"atom({})", atomid));
                //MessageBox(0,resid.c_str(), 0, 0);
                //MessageBox(0,copy.c_str(), 0, 0);
                //MessageBox(0, L"shet", 0, 0);
                //DirectUI::DUIXmlParser* parser = 0;
                //DirectUI::DUIXmlParser::Create(&parser, 0, 0, 0, 0);

                //HRESULT hr = pDuiManager->pParser->SetXMLFromResource((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_SECURITYOPTION),pDuiManager->hInstance,pDuiManager->hInstance);
                //if (FAILED(hr))
                //{
                //    MessageBox(0, L"setxml failed", 0, 0);
                //    continue;
                //}
                const wchar_t* resid = (i == buttonList.size() - 1) ? L"securitycancelid" : L"securityoptionid";
                DirectUI::Button* btn = 0;
                HRESULT hr = pDuiManager->pParser->CreateElement(
                    (DirectUI::UCString)resid,
                    NULL,
                    NULL,
                    NULL,
                    (DirectUI::Element**)&btn
                );

                //if (btn->GetParent() != SecurityOptionsContainer)
                //    MessageBox(0,L"Parent mismatch",0,0);

                if (!btn)
                {
                    MessageBox(0, L"CreateElement failed", 0, 0);
                    continue;
                }
                
                //btn->SetSheet(sheet);
                auto stringelement = btn->FindDescendent(ATOMID(L"atomid"));     
                if (!stringelement)                                              
                {                                                                
                    stringelement = btn;
                }                                                                
                std::wstring str = buttonwrapper.getString();                    
                                                                                 
                stringelement->SetContentString((DirectUI::UCString)str.c_str());
                stringelement->SetContentAlign(5);                               
                btn->SetID((DirectUI::UCString)std::to_wstring(i).c_str());

                auto parent = (i == buttonList.size() - 1) ? DialogButtonFrame : SecurityOptionsContainer;

                DirectUIElementAdd(parent,btn);


                //SecurityOptionsContainer->Add((DirectUI::Element**)&btn, 1);

                //DirectUI::Layout* flowlayout = NULL;
                //DirectUI::FlowLayout::Create(0,0,0,0,(DirectUI::Layout**)&flowlayout);
                //btn->SetLayout(flowlayout);

                //if (i == buttonList.size() - 1)
                //    btn->SetClass((DirectUI::UCString)L"GenericButton");
                //else
                //    btn->SetClass((DirectUI::UCString)L"SecurityOptions");
                //
                //if (i == buttonList.size() - 1)
                //    stringelement->SetClass((DirectUI::UCString)L"CancelText");
                //else
                //    stringelement->SetClass((DirectUI::UCString)L"SecurityOptions");
                //btn->SetSheet(sheet);
                //stringelement->SetSheet(sheet);

            }
            DumpDuiTree(SecurityOptionsContainer, 0);
        }, &buttonsList);
}

void external::SecurityOptionControl_Create(void* actualInstance)
{
    SecurityOptionControlWrapper button(actualInstance);
    SPDLOG_INFO("SecurityOptionControl_Create {} {}",(void*)actualInstance, ws2s(button.getString()));

    buttonsList.push_back(button);
}

void external::SecurityOptionControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("FOUND AND DELETING BUTTON");
            buttonsList.erase(buttonsList.begin() + i);
            break;
        }
    }
}

void external::SecurityControl_SetInactive()
{
    //auto securityControl = duiManager::Get()->GetWindowOfTypeId<uiSecurityControl>(2);
    //if (securityControl)
    //{
    //    SPDLOG_INFO("setting inactive security control instance");
    //    securityControl->SetInactive();
    //}
}

void SecurityOptionControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = VK_RETURN;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling SecurityOptionControl_Press with enter on the control!", actualInstance);

        external::SecurityOptionControl_Press(actualInstance, &keyrecord, &result);
    }
}

std::wstring SecurityOptionControlWrapper::getString()
{
    return external::SecurityOptionControl_getString(actualInstance);
}

DirectUI::IClassInfo* duiSecurityControl::Class = NULL;

duiSecurityControl::duiSecurityControl()
{
}

duiSecurityControl::~duiSecurityControl()
{
}

void duiSecurityControl::OnInput(DirectUI::InputEvent* a2)
{
    if (a2->device == DirectUI::GINPUT_KEYBOARD)
    {
        if (reinterpret_cast<DirectUI::KeyboardEvent*>(a2)->ch == VK_ESCAPE)
        {
            KEY_EVENT_RECORD rec;
            rec.wVirtualKeyCode = VK_ESCAPE; //forward it to consoleuiview
            external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
        }
    }
    DirectUI::Element::OnInput(a2);
}

HRESULT duiSecurityControl::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiSecurityControl* instance = (duiSecurityControl*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiSecurityControl));

    if (instance != NULL)
    {
        new (instance) duiSecurityControl();
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

DirectUI::IClassInfo* duiSecurityControl::GetClassInfoW()
{
    return duiSecurityControl::Class;
}

void duiSecurityControl::OnEvent(DirectUI::Event* iev)
{
    if (!iev->target || !iev->target->GetParent()) return DirectUI::Element::OnEvent(iev);

    if (iev->target->GetParent()->GetID() == ATOMID(L"SecurityOptionsContainer") && iev->type == DirectUI::Button::Click)
    {
        //minus 1 on index cuz there is a placeholder button, because for some reason first element of the verticalflowlayout is fucked, and offscreen
        int index = iev->target->GetIndex() - 1;
        if (index >= 0 && index < buttonsList.size())
        {
            auto& wrapper = buttonsList[index];
            if (wrapper.actualInstance)
                wrapper.Press();
        }
        //auto id = iev->target->GetIndex();
        //MessageBoxW(0, std::to_wstring(id).c_str(), 0, 0);
    }
    if (iev->target->GetParent()->GetID() == ATOMID(L"DialogButtonFrame") && iev->type == DirectUI::Button::Click) //cancel button
    {
        int index = buttonsList.size() - 1;
        if (index >= 0 && index < buttonsList.size())
        {
            auto& wrapper = buttonsList[index];
            if (wrapper.actualInstance)
                wrapper.Press();
        }
    }
    DirectUI::Element::OnEvent(iev);
}

void duiSecurityControl::OnDestroy()
{
    //MessageBoxW(0,L"destroy", L"destroy",0);
    for (int i = createdTexts.size() - 1; i >= 0; --i)
    {
        auto element = createdTexts[i];
        if (element && !IsBadReadPtr(element, 8))
            element->Destroy(true);

        createdTexts.erase(createdTexts.begin() + i);
    }

    for (int i = createdButtons.size() - 1; i >= 0; --i)
    {
        auto element = createdButtons[i];
        if (element && !IsBadReadPtr(element, 8))
            element->Destroy(true);

        createdButtons.erase(createdButtons.begin() + i);
    }
    
    DirectUI::Element::OnDestroy();
}

void duiSecurityControl::Begin()
{

}

/*HRESULT securityButtonControl::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    return E_NOTIMPL;
}

DirectUI::IClassInfo* securityButtonControl::GetClassInfoW()
{
    return nullptr;
}*/
