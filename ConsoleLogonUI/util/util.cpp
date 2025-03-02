#include "util.h"
#include "ui/dui_manager.h"
#include <wincodec.h>
#include <wincodecsdk.h>

HBITMAP GetHBITMAPFromImageResource(int resourceID)
{
	static auto SHCreateStreamOnModuleResourceW = (HRESULT(WINAPI*)(HMODULE hModule, LPCWSTR pwszName, LPCWSTR pwszType, IStream * *ppStream))(GetProcAddress(LoadLibrary(L"shcore.dll"), (PCSTR)109));

	IStream* Stream = 0;
	HRESULT hr = SHCreateStreamOnModuleResourceW(duiManager::Get()->hInstance, MAKEINTRESOURCEW(resourceID), L"IMAGE", &Stream);
	if (FAILED(hr))
	{
		return 0;
	}
	Gdiplus::GdiplusStartupInput gpStartupInput;
	ULONG_PTR gpToken;
	Gdiplus::GdiplusStartup(&gpToken, &gpStartupInput, NULL);

	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream(Stream);

	HBITMAP hbitmap = 0;

	if (bitmap)
	{
		UINT newWidth = GetSystemMetrics(SM_CXSCREEN);
		UINT newHeight = GetSystemMetrics(SM_CYSCREEN);
		Gdiplus::Bitmap bitmap2 = Gdiplus::Bitmap(newWidth,newHeight,PixelFormat32bppARGB);

		Gdiplus::Graphics* graphic = Gdiplus::Graphics::FromImage(&bitmap2);
		if (graphic)
		{
			graphic->SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);

			Gdiplus::ImageAttributes attributes;
			attributes.SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY, 0xFF000000, 0);

			UINT oldWidth = bitmap->GetWidth();
			UINT oldHeight = bitmap->GetHeight();

			graphic->DrawImage(bitmap, Gdiplus::Rect(0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), 0,0, bitmap->GetWidth(), bitmap->GetHeight(), Gdiplus::Unit::UnitPixel, &attributes, (Gdiplus::DrawImageAbort)0, 0);

			bitmap2.GetHBITMAP(Gdiplus::Color(0xFF000000), &hbitmap);

			delete graphic;
		}

		delete bitmap;
	}
	Stream->Release();
	Gdiplus::GdiplusShutdown(gpToken);

	return hbitmap;
}
