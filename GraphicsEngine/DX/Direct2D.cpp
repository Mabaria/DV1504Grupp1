#include "Direct2D.h"

Direct2D::Direct2D()
{
	this->pFactory = nullptr;
	this->pRenderTarget = nullptr;
	this->pWicFactory = nullptr;
	this->pConverter = nullptr;
	this->pDecoder = nullptr;
	this->pBitmapSrc = nullptr;

	this->Init();
}

Direct2D::~Direct2D()
{
}

void Direct2D::CreateFactory()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->pFactory);
}

void Direct2D::Init()
{
	this->CreateFactory();
	this->CreateWicFactory();
}

void Direct2D::CreateRenderTarget(
	HWND window,
	unsigned int width,
	unsigned int height)
{
	this->pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			window,
			D2D1::SizeU(width, height)),
		&this->pRenderTarget);
}

IWICFormatConverter *Direct2D::GetpFormatConverter()
{
	return this->pConverter;
}

IWICImagingFactory *Direct2D::GetpImagingFactory()
{
	return this->pWicFactory;
}

IWICBitmapDecoder *Direct2D::GetpBitmapDecoder()
{
	return this->pDecoder;
}

IWICBitmapFrameDecode *Direct2D::GetpBitmapSrc()
{
	return this->pBitmapSrc;
}

ID2D1Factory *Direct2D::GetpFactory()
{
	return this->pFactory;
}

ID2D1HwndRenderTarget *Direct2D::GetpRenderTarget()
{
	return this->pRenderTarget;
}

void Direct2D::SetpFormatConverter(IWICFormatConverter* pConverter)
{
	this->pConverter = pConverter;
}

void Direct2D::SetpImagingFactory(IWICImagingFactory *pWicFactory)
{
	this->pWicFactory = pWicFactory;
}

void Direct2D::SetpBitmapDecoder(IWICBitmapDecoder * pDecoder)
{
	this->pDecoder = pDecoder;
}

void Direct2D::SetpBitmapSrc(IWICBitmapFrameDecode * pBitmapSrc)
{
	this->pBitmapSrc = pBitmapSrc;
}

void Direct2D::CreateWicFactory()
{
	CoInitialize(nullptr);
	CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->pWicFactory));
}