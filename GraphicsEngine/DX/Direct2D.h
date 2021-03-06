#pragma once
#include <d2d1.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <vector>
#include <Windows.h>
#include <wincodec.h>
#include <dwrite.h>
#include "../CustomTextRenderer.h"
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "windowscodecs")
#pragma comment(lib, "d2d1")
#pragma comment (lib, "d3d11.lib")

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
	IDWriteFactory *GetpTextFactory();
	const DWRITE_TRIMMING GetTrimmer();

	ID2D1DeviceContext* GetpContext();
	
	CustomTextRenderer *GetpTextRenderer();

	void SetpFormatConverter(IWICFormatConverter* pConverter);
	void SetpImagingFactory(IWICImagingFactory *pWicFactory);
	void SetpBitmapDecoder(IWICBitmapDecoder *pDecoder);
	void SetpBitmapSrc(IWICBitmapFrameDecode *pBitmapSrc);
	void InitDeviceAndContext(IDXGIDevice *dxgiDevice);

	void DrawLine(
		float startX,
		float startY,
		float endX,
		float endY,
		float lineThickness,
		float r,
		float g,
		float b,
		float a = 1.f);
	void DrawRect(
		float left,
		float top,
		float right,
		float bottom,
		float lineThickness,
		float r,
		float g,
		float b,
		float a = 1.f);
private:

	IWICFormatConverter * mpConverter;
	IWICImagingFactory *mpWicFactory;
	IWICBitmapDecoder *mpDecoder;
	IWICBitmapFrameDecode *mpBitmapSrc;
	ID2D1HwndRenderTarget *mpRenderTarget;
	ID2D1Factory *mpFactory;

	IDWriteFactory *mpTextFactory;
	DWRITE_TRIMMING mTrimmer;

	ID2D1Factory1 *mpFactory1;
	ID2D1Device *mpDevice;
	ID2D1DeviceContext *mpContext;


	CustomTextRenderer* mpTextRenderer;

	void mCreateFactory();
	void mCreateWicFactory();
	void mCreateTextFactory();
	void mCreateTextRenderer();
	void mInit();
	void ReleaseCOM(IUnknown *object);
};