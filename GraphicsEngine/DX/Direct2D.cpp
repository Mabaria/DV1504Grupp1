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
	this->mpTextFactory = nullptr;
	this->mpTextFormat = nullptr;
	this->mTrimmer = {};
	this->mFontSize = 12;
	this->mTrimmer.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;

	this->mInit();
	this->CreateRenderTarget(window, width, height);	
}

Direct2D::~Direct2D()
{
	this->ReleaseCOM(this->mpFactory);
	this->ReleaseCOM(this->mpRenderTarget);
	this->ReleaseCOM(this->mpWicFactory);
	this->ReleaseCOM(this->mpConverter);
	this->ReleaseCOM(this->mpDecoder);
	this->ReleaseCOM(this->mpBitmapSrc);
	this->ReleaseCOM(this->mpTextFactory);
	this->ReleaseCOM(this->mpTextFormat);
}

void Direct2D::mCreateFactory()
{
	 D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->mpFactory);
}

void Direct2D::mInit()
{
	this->mCreateFactory();
	this->mCreateWicFactory();
	this->mCreateTextFactory();
	this->mCreateTextFormat();
	this->mpTextFormat->SetTrimming(&this->mTrimmer, NULL);
	this->mpTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);
}

void Direct2D::ReleaseCOM(IUnknown * object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
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

IDWriteFactory *Direct2D::GetpTextFactory()
{
	return this->mpTextFactory;
}

IDWriteTextFormat *Direct2D::GetpTextFormat()
{
	return this->mpTextFormat;
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

void Direct2D::SetFontSize(int size)
{
	this->mFontSize = size;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();
}

void Direct2D::mCreateWicFactory()
{
	CoInitialize(nullptr);
	CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->mpWicFactory));
}

void Direct2D::mCreateTextFactory()
{
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&this->mpTextFactory));
}

void Direct2D::mCreateTextFormat()
{
	HRESULT hr = this->mpTextFactory->CreateTextFormat(
		L"Times new roman",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		this->mFontSize,
		L"sv-SE",
		&this->mpTextFormat
	);
}

