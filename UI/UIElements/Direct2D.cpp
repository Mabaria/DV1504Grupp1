#include "Direct2D.h"

Direct2D::Direct2D()
{
	this->pFactory = NULL;
	this->pWicFactory = NULL;
	this->pDecoder = NULL;
	this->pBitmapSrc = NULL;
	this->pConverter = NULL;
}

Direct2D::~Direct2D()
{
}

void Direct2D::CreateFactory()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&this->pFactory);
}

void Direct2D::CreateWicFactory()
{	
	CoInitialize(nullptr);
	CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->pWicFactory));
}

void Direct2D::Init()
{
	this->CreateFactory();
	this->CreateWicFactory();
	
}

void Direct2D::CreateRenderTarget(HWND window, unsigned int width, unsigned int height)
{
	this->pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			window,
			D2D1::SizeU(width, height)),
		&this->pRenderTarget);
}

void Direct2D::LoadImageToBitmap(LPCWSTR imageFilePath)
{
	ID2D1Bitmap *temp_bitmap;
	std::string hehe;

	this->pWicFactory->CreateFormatConverter(&this->pConverter);
	this->pWicFactory->CreateDecoderFromFilename(imageFilePath,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&this->pDecoder);
	this->pDecoder->GetFrame(0, &this->pBitmapSrc);
	this->pConverter->Initialize(this->pBitmapSrc,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut);

	/*this->pRenderTarget->CreateBitmapFromWicBitmap(this->pConverter, 
		NULL, 
		&this->mImages[0])*/
}

