#include "Direct2D.h"

Direct2D::Direct2D(HWND window,
	unsigned int width,
	unsigned int height)
{
	this->mpFactory = nullptr;
	this->mpRenderTarget = nullptr;
	this->mpWicFactory = nullptr;
	this->mpConverter = nullptr;
	this->mpDecoder = nullptr;
	this->mpBitmapSrc = nullptr;

	this->mCreateFactory();
	this->mCreateWicFactory();
	this->CreateRenderTarget(window, width, height);
	
	//this->mInit();
	
}

Direct2D::~Direct2D()
{
}

void Direct2D::mCreateFactory()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->mpFactory);
}

void Direct2D::mInit()
{
	this->mCreateFactory();
	this->mCreateWicFactory();
}

void Direct2D::CreateRenderTarget(
	HWND window,
	unsigned int width,
	unsigned int height)
{
	HRESULT hr = this->mpFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			window,
			D2D1::SizeU(width, height)),
		&this->mpRenderTarget);
}

IWICFormatConverter *Direct2D::GetpFormatConverter()
{
	return this->mpConverter;
}

IWICImagingFactory *Direct2D::GetpImagingFactory()
{
	return this->mpWicFactory;
}

IWICBitmapDecoder *Direct2D::GetpBitmapDecoder()
{
	return this->mpDecoder;
}

IWICBitmapFrameDecode *Direct2D::GetpBitmapSrc()
{
	return this->mpBitmapSrc;
}

ID2D1Factory *Direct2D::GetpFactory()
{
	return this->mpFactory;
}

ID2D1HwndRenderTarget *Direct2D::GetpRenderTarget()
{
	return this->mpRenderTarget;
}

void Direct2D::SetpFormatConverter(IWICFormatConverter* pConverter)
{
	this->mpConverter = pConverter;
}

void Direct2D::SetpImagingFactory(IWICImagingFactory *pWicFactory)
{
	this->mpWicFactory = pWicFactory;
}

void Direct2D::SetpBitmapDecoder(IWICBitmapDecoder * pDecoder)
{
	this->mpDecoder = pDecoder;
}

void Direct2D::SetpBitmapSrc(IWICBitmapFrameDecode * pBitmapSrc)
{
	this->mpBitmapSrc = pBitmapSrc;
}

void Direct2D::mCreateWicFactory()
{
	CoInitialize(nullptr);
	CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->mpWicFactory));
}