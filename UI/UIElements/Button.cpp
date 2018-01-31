#include "Button.h"

Button::Button()
{
	this->pBitMap = nullptr;
	this->pBitmapSrc = nullptr;
	this->pConverter = nullptr;
	this->pDecoder = nullptr;
	this->pWicFactory = nullptr;
}

Button::~Button()
{
}

const wchar_t * Button::StrToWchar(std::string str)
{
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > convert;
	std::wstring wstr = convert.from_bytes(str);
	const wchar_t* result = wstr.c_str();
	return result;
}

void Button::CreateButton(ID2D1HwndRenderTarget * renderTarget, std::string imageFilePath, int left, int top, int right, int bottom)
{
	this->LoadImageToBitmap(renderTarget, imageFilePath);
	this->mButtonSize = D2D1::RectF(left, top, right, bottom);
	this->mBitmapRenderSize = D2D1::RectF(
		0, 
		0, 
		this->pBitMap->GetSize().width, 
		this->pBitMap->GetSize().height);

	/*renderTarget->DrawBitmap(
		this->pBitMap,
		this->mButtonSize,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		this->mBitmapRenderSize);*/
}

ID2D1Bitmap* Button::getBitmapPointer()
{
	return this->pBitMap;
}

void Button::CreateWicFactory()
{
	CoInitialize(nullptr);
	CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->pWicFactory));
}

void Button::LoadImageToBitmap(
	ID2D1HwndRenderTarget *renderTarget, 
	std::string imageFilePath)
{
	this->mFilePath = imageFilePath;

	this->pWicFactory->CreateFormatConverter(&this->pConverter);
	this->pWicFactory->CreateDecoderFromFilename(
		this->StrToWchar(imageFilePath),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&this->pDecoder);
	this->pDecoder->GetFrame(0, &this->pBitmapSrc);
	this->pConverter->Initialize(
		this->pBitmapSrc,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut);

	renderTarget->CreateBitmapFromWicBitmap(this->pConverter,
		NULL,
		&this->pBitMap);
	this->pConverter->Release();
}

