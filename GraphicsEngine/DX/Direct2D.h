#pragma once
#include <d2d1.h>
#include <vector>
#include <Windows.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs")
#pragma comment(lib, "d2d1")

class Direct2D
{
public:
	Direct2D();
	~Direct2D();
	void CreateRenderTarget(HWND window, unsigned int width, unsigned int height);
	 IWICFormatConverter *GetpFormatConverter();
	 IWICImagingFactory *GetpImagingFactory();
	 IWICBitmapDecoder *GetpBitmapDecoder();
	 IWICBitmapFrameDecode *GetpBitmapSrc();
	 ID2D1Factory *GetpFactory();
	 ID2D1HwndRenderTarget *GetpRenderTarget();

	void SetpFormatConverter(IWICFormatConverter* pConverter);
	void SetpImagingFactory(IWICImagingFactory *pWicFactory);
	void SetpBitmapDecoder(IWICBitmapDecoder *pDecoder);
	void SetpBitmapSrc(IWICBitmapFrameDecode *pBitmapSrc);
private:

	IWICFormatConverter *pConverter;
	IWICImagingFactory *pWicFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pBitmapSrc;
	ID2D1HwndRenderTarget *pRenderTarget;
	ID2D1Factory *pFactory;

	void CreateFactory();
	void CreateWicFactory();
	void Init();
};