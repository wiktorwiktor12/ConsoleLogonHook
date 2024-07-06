#include "util.h"
#include "ui/dui_manager.h"
#include <wincodec.h>
#include <wincodecsdk.h>

HBITMAP GetHBITMAPFromImageResource(int resourceID)
{
    auto foundresource = FindResourceW(duiManager::Get()->hInstance, MAKEINTRESOURCEW(resourceID), L"IMAGE");
    if (!foundresource) return (HBITMAP)0;
    DWORD imageSize = SizeofResource(duiManager::Get()->hInstance, foundresource);
    const void* resourceData = ::LockResource(::LoadResource(duiManager::Get()->hInstance, foundresource));

    HBITMAP hbitmap = 0;

    void* buffer = GlobalAlloc(GMEM_MOVEABLE,imageSize);
    if (buffer)
    {
        void* pBuffer = ::GlobalLock(buffer);
        memcpy(pBuffer, resourceData, imageSize);
        IStream* pStream = NULL;
        if (CreateStreamOnHGlobal(buffer, FALSE, &pStream) == S_OK)
        {
            Gdiplus::GdiplusStartupInput gpStartupInput;
            ULONG_PTR gpToken;
            Gdiplus::GdiplusStartup(&gpToken, &gpStartupInput, NULL);

            Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream(pStream);

            if (bitmap)
            {
                bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &hbitmap);
                pStream->Release();
                delete bitmap;
            }
            Gdiplus::GdiplusShutdown(gpToken);
        }
        GlobalUnlock(buffer);
        GlobalFree(buffer);
    }
    
    return hbitmap;
}
