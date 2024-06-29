#include "util.h"
#include <wincodec.h>
#include <wincodecsdk.h>

HBITMAP GetHBITMAPFromResouce(int resource)
{
    HRESULT hr = S_OK;

    // WIC interface pointers.
    IWICStream* pIWICStream = NULL;
    IWICBitmapDecoder* pIDecoder = NULL;
    IWICBitmapFrameDecode* pIDecoderFrame = NULL;

    // Resource management.
    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void* pImageFile = NULL;
    DWORD imageFileSize = 0;

    imageResHandle = FindResource(
        NULL,             // This component.
        MAKEINTRESOURCE(resource),   // Resource name.
        L"Bitmap");        // Resource type.

    hr = (imageResHandle ? S_OK : E_FAIL);

    // Load the resource to the HGLOBAL.
    if (SUCCEEDED(hr)) {
        imageResDataHandle = LoadResource(NULL, imageResHandle);
        hr = (imageResDataHandle ? S_OK : E_FAIL);
    }
    if (SUCCEEDED(hr)) {
        pImageFile = LockResource(imageResDataHandle);
        hr = (pImageFile ? S_OK : E_FAIL);
    }
    if (SUCCEEDED(hr)) {
        imageFileSize = SizeofResource(NULL, imageResHandle);
        hr = (imageFileSize ? S_OK : E_FAIL);
    }
    CComPtr<IWICImagingFactory> m_pIWICFactory;
    // Create WIC factory
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pIWICFactory)
    );

    if (SUCCEEDED(hr)) {
        hr = m_pIWICFactory->CreateStream(&pIWICStream);
    }

    // Initialize the stream with the memory pointer and size.
    if (SUCCEEDED(hr)) {
        hr = pIWICStream->InitializeFromMemory(
            reinterpret_cast<BYTE*>(pImageFile),
            imageFileSize);
    }

    // Create a decoder for the stream.
    if (SUCCEEDED(hr)) {
        hr = m_pIWICFactory->CreateDecoderFromStream(
            pIWICStream,                   // The stream to use to create the decoder
            NULL,                          // Do not prefer a particular vendor
            WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
            &pIDecoder);                   // Pointer to the decoder
    }
    // Retrieve the first bitmap frame.
    if (SUCCEEDED(hr))
    {
        hr = pIDecoder->GetFrame(0, &pIDecoderFrame);
    }
    UINT width = 0, height = 0;
    pIDecoderFrame->GetSize(&width, &height);

    std::vector<BYTE> buffer(width * height * 4);
    pIDecoderFrame->CopyPixels(0, width * 4, buffer.size(), buffer.data());

    HBITMAP bitmap = CreateBitmap(width, height, 1, 32, buffer.data());
    return bitmap;
    //IWICFormatConverter* pConverter = nullptr;
    //m_pIWICFactory->CreateFormatConverter(&pConverter);
    //pConverter->Initialize(pIDecoderFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
    //
    //HBITMAP hBitmap = nullptr;
    //m_pIWICFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, reinterpret_cast<IWICBitmap**>(&hBitmap));
}
