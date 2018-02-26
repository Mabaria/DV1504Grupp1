#include "Direct2D.h"

Direct2D::Direct2D()
{
	this->mpFactory = nullptr;
	this->mpRenderTarget = nullptr;
	this->mpWicFactory = nullptr;
	this->mpConverter = nullptr;
	this->mpDecoder = nullptr;
	this->mpBitmapSrc = nullptr;
	this->mpTextFactory = nullptr;
	this->mpFactory1 = nullptr;
	this->mpDevice = nullptr;
	this->mpContext = nullptr;
	this->mTrimmer = {};
	this->mTrimmer.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
	this->mInit();
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
	this->ReleaseCOM(this->mpFactory1);
	this->ReleaseCOM(this->mpDevice);
	this->ReleaseCOM(this->mpContext);
	this->ReleaseCOM(this->mpTextRenderer);
	delete this->mpTextRenderer;
}

void Direct2D::mCreateFactory()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->mpFactory);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->mpFactory1);
}

void Direct2D::mInit()
{
	this->mCreateFactory();
	this->mCreateWicFactory();
	this->mCreateTextFactory();
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
	this->mpRenderTarget->SetAntialiasMode
	(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	this->mCreateTextRenderer(); // Render target needs to exist
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

const DWRITE_TRIMMING Direct2D::GetTrimmer()
{
	return this->mTrimmer;
}

ID2D1DeviceContext * Direct2D::GetpContext()
{
	return this->mpContext;
}


CustomTextRenderer * Direct2D::GetpTextRenderer()
{
	return this->mpTextRenderer;
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

void Direct2D::InitDeviceAndContext(IDXGIDevice * dxgiDevice)
{
	this->mpFactory1->CreateDevice(dxgiDevice, &this->mpDevice);
	this->mpDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&this->mpContext);
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

void Direct2D::mCreateTextRenderer()
{
	this->mpTextRenderer = nullptr;
	ID2D1SolidColorBrush  *outlineBrush, *fillBrush;

	// Create outline brush
	this->mpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &outlineBrush);
	// Create solid color fill brush
	this->mpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White), &fillBrush);


	this->mpTextRenderer = new CustomTextRenderer(
		this->mpFactory,
		this->mpRenderTarget,
		outlineBrush,
		fillBrush
	);

	// Release the locally created objects, text renderer has its own ref
	outlineBrush->Release();
	fillBrush->Release();
}
