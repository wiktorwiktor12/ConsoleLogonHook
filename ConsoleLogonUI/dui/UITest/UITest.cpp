#include <Windows.h>

#include <Vsstyle.h>
#include <vssym32.h>

#include <wil/common.h>
#include <wil/result.h>

#include <fstream>
#include <sstream>
#include <format>
#include <functional>
#include <filesystem>

#include "..\DirectUI\DirectUI.h"

#pragma comment(lib,"dui70.lib")
#pragma comment(lib, "comctl32.lib")
#include "resource.h"

using namespace DirectUI;

struct LogListener : public IElementListener {

	//0
	void OnListenerAttach(Element *elem) override {
		OutputDebugString(std::format(L"attach: {:p}\n", (void*)elem).c_str());
	}
	//1
	void OnListenerDetach(Element *elem) override {
		OutputDebugString(std::format(L"detach: {:p}\n", (void*)elem).c_str());
	}
	//2
	bool OnPropertyChanging(Element* elem, const PropertyInfo *prop, int unk, Value* v1, Value* v2) override {
		OutputDebugString(std::format(L"prop change: {:p} {} {} {:p}<{}> {:p}<{}>\n",
			(void*)elem, (PCWSTR)prop->name, unk, (void*)v1, v1->GetType(), (void*)v2, v2->GetType()).c_str());
		return true;
	}
	//3
	void OnListenedPropertyChanged(Element *elem, const PropertyInfo*prop, int type, Value*v1, Value*v2) override {
		OutputDebugString(std::format(L"listened prop change: {:p} {} {} {:p}<{}> {:p}<{}>\n",
			(void*)elem, (PCWSTR)prop->name, type, (void*)v1, v1->GetType(), (void*)v2, v2->GetType()).c_str());
	}
	//4
	void OnListenedEvent(Element*elem, struct Event*ev) override {
		 OutputDebugString(std::format(L"listened event: {:p} {:p}\n", (void*)elem, (void*)ev).c_str());
	}
	//5
	void OnListenedInput(Element*elem, struct InputEvent*iev) override {
		OutputDebugString(std::format(L"listened input: {:p} {:p}\n", (void*)elem, (void*)iev).c_str());
	}

};


struct EventListener : public IElementListener {

	using handler_t = std::function<void(Element*, Event*)>;

	handler_t f;

	EventListener(handler_t f) : f(f) { }

	void OnListenerAttach(Element *elem) override { }
	void OnListenerDetach(Element *elem) override { }
	bool OnPropertyChanging(Element* elem, const PropertyInfo *prop, int unk, Value* v1, Value* v2) override {
		return true;
	}
	void OnListenedPropertyChanged(Element *elem, const PropertyInfo*prop, int type, Value*v1, Value*v2) override { }
	void OnListenedEvent(Element*elem, struct Event*iev) override {
		f(elem, iev);
	}
	void OnListenedInput(Element*elem, struct InputEvent*ev) override { }
};

std::wstring to_string(ValueType type) {
	switch (type) {
		case ValueType::Unavailable: return L"Unavailable";
    case ValueType::Unset: return L"Unset";
    case ValueType::Null: return L"Null";
    case ValueType::Int: return L"Int";
    case ValueType::Bool: return L"Bool";
    case ValueType::Element: return L"Element";
    case ValueType::Ellist: return L"Ellist";
    case ValueType::String: return L"String";
    case ValueType::Point: return L"Point";
    case ValueType::Size: return L"Size";
    case ValueType::Rect: return L"Rect";
    case ValueType::Color: return L"Color";
    case ValueType::Layout: return L"Layout";
		case ValueType::Graphic: return L"Graphic";
    case ValueType::Sheet: return L"Sheet";
    case ValueType::Expr: return L"Expr";
		case ValueType::Atom: return L"Atom";
    case ValueType::Cursor: return L"Cursor";
    case ValueType::Float: return L"Float";
		case ValueType::DblList: return L"DblList";
    default:
			throw std::logic_error{"unreachable"};
	}
}

void DumpClassInfo(IClassInfo *info) {

	constexpr auto DOC_DIR = LR"(C:\Users\7mile\source\repos\DirectUI\docs)";

	std::wstring name = (LPCWSTR)info->GetName();

	std::wofstream os{std::filesystem::path{DOC_DIR} / (name + L"Class.g.txt")};

  os << (std::format(L"ClassInfo: <{}>\n", name).c_str());
	
	auto *base = info->GetBaseClass();
	os << (std::format(L"  Base Class: <{}>\n", 
		base ? (LPCWSTR)base->GetName() : L"None").c_str());

	os << (L"  Properties:\n");
	for (unsigned int i = 0; i < info->GetPICount(); i++) {
		auto prop = info->EnumPropertyInfo(i);
		os << (std::format(L"    [{}]: {}\n",
			(LPCWSTR)prop->name, to_string(prop->cap->type)).c_str());
		if (prop->enum_value_map) {
			 os << (L"      Enum values:\n");
      for (auto *ptr = prop->enum_value_map; ptr->str_value; ptr++) {
			  os << (std::format(L"        {} : 0x{:x} ({})\n",
					(PCWSTR)ptr->str_value, (UINT32)ptr->int_value, ptr->int_value).c_str());
			}
		}
	}



}

long (* RealClassFactoryRegister)(CClassFactory *, IClassInfo*) = 0;

HRESULT HookedRegister(CClassFactory *self, IClassInfo*info) {
	DumpClassInfo(info);
	return RealClassFactoryRegister(self, info);
}

inline void HookClassFactoryRegister() {
	RealClassFactoryRegister = (decltype(RealClassFactoryRegister))(
		(UINT64)GetModuleHandle(L"dui70.dll") +
		0x37634
	);

	//DetourTransactionBegin();
	//DetourUpdateThread(GetCurrentThread());

	auto pfMine = &HookedRegister;
	//DetourAttach(&(PVOID&)RealClassFactoryRegister,
    //             *(PBYTE*)&pfMine);
	//DetourTransactionCommit();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	CoInitializeEx(NULL, 0);

	InitProcessPriv(14, NULL, 0, true);
	InitThread(2);

	// uncomment to update class definitions
	// HookClassFactoryRegister();
        RegisterAllControls();


	NativeHWNDHost* pwnd;

	NativeHWNDHost::Create((UCString)L"Microsoft DirectUI Test", NULL, NULL,
		//CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		600, 400, 800, 600,
		WS_EX_WINDOWEDGE, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,&pwnd);

	DUIXmlParser* pParser;

	DUIXmlParser::Create(&pParser, NULL, NULL, NULL, NULL);

	pParser->SetParseErrorCallback(
            [](UCString err1, UCString err2, int unk, void *ctx) {
              MessageBox(NULL,
                         std::format(L"err: {}; {}; {}\n", (LPCWSTR)err1,
                                     (LPCWSTR)err2, unk)
                             .c_str(),
                         L"Error while parsing DirectUI", 0);
		DebugBreak();
	}, NULL);

	auto hr=pParser->SetXMLFromResource(IDR_UIFILE1, hInstance,(HINSTANCE)hInstance);

	unsigned long defer_key;
	HWNDElement* hwnd_element;

	HWNDElement::Create(pwnd->GetHWND(),true,0,NULL,&defer_key,(Element**)&hwnd_element);

	Element* pWizardMain;
	hr = pParser->CreateElement((UCString)L"WizardMain", hwnd_element,NULL,NULL,(Element**)&pWizardMain);



	pWizardMain->SetVisible(true);
	pWizardMain->EndDefer(defer_key);
	pwnd->Host(pWizardMain);

	pwnd->ShowWindow(SW_SHOW);

	//DumpDuiTree(pWizardMain, 0);

	StartMessagePump();

	UnInitProcessPriv(NULL);
	return 0;
}
