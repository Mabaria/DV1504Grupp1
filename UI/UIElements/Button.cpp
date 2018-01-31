#include "Button.h"

Button::Button()
{
	this->pBitMap = nullptr;
	this->mFilePath = "";
}

Button::~Button()
{
}

const wchar_t* Button::StrToWchar(std::string str) // FIX
{
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > convert;
	std::wstring wstr = convert.from_bytes(str);
	const wchar_t *result = wstr.c_str();
	this->mFilePathAsWstr= wstr.c_str();
	return result;
}

void Button::CreateButton(
	Direct2D D2D1Panel, 
	std::string imageFilePath, 
	int left, 
	int top, 
	int right, 
	int bottom)
{
	this->LoadImageToBitmap(D2D1Panel, imageFilePath);
	this->mButtonSize = D2D1::RectF(left, top, right, bottom);
	this->mBitmapRenderSize = D2D1::RectF(
		0, 
		0, 
		this->pBitMap->GetSize().width, 
		this->pBitMap->GetSize().height);
}

void Button::DrawButton(Direct2D D2D1Panel)
{
	D2D1Panel.GetpRenderTarget()->DrawBitmap(
	this->pBitMap,
	this->mButtonSize,
	1.0f,
	D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
	this->mBitmapRenderSize);
}

ID2D1Bitmap* Button::getBitmapPointer()
{
	return this->pBitMap;
}

const D2D1_RECT_F Button::GetButtonsize() const
{
	return this->mButtonSize;
}

const D2D1_RECT_F Button::GetBitmapRenderSize() const
{
	return this->mBitmapRenderSize;
}

void Button::SetButtonsize(int left, int top, int right, int bottom)
{
	this->mButtonSize = D2D1::RectF(left, top, right, bottom);
}

void Button::SetBitmapRendersize(int left, int top, int right, int bottom)
{
	this->mBitmapRenderSize = D2D1::RectF(left, top, right, bottom);
}

void Button::SetButtonStatus(BUTTON_STATE buttState)
{
	
}

void Button::LoadImageToBitmap(
	Direct2D D2D1Panel,
	std::string imageFilePath)
{
	this->mFilePath = imageFilePath;
	this->StrToWchar(imageFilePath);
	const wchar_t *p_file_path_wchar = this->mFilePathAsWstr.c_str();
	IWICFormatConverter *converter = D2D1Panel.GetpFormatConverter();
	IWICBitmapDecoder *decoder = D2D1Panel.GetpBitmapDecoder();
	IWICBitmapFrameDecode *bitmapSrc = D2D1Panel.GetpBitmapSrc();

	D2D1Panel.GetpImagingFactory()->CreateFormatConverter(&converter);
	D2D1Panel.SetpFormatConverter(converter);
	D2D1Panel.GetpImagingFactory()->CreateDecoderFromFilename(
		p_file_path_wchar,
		NULL, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	D2D1Panel.SetpBitmapDecoder(decoder);
	D2D1Panel.GetpBitmapDecoder()->GetFrame(0, &bitmapSrc);
	D2D1Panel.SetpBitmapSrc(bitmapSrc);
	D2D1Panel.GetpFormatConverter()->Initialize(
		bitmapSrc,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut);
	D2D1Panel.GetpRenderTarget()->CreateBitmapFromWicBitmap(
		D2D1Panel.GetpFormatConverter(),
		NULL,
		&this->pBitMap);
	D2D1Panel.GetpFormatConverter()->Release();
	this->mBitmapRenderSize = D2D1::RectF(
		0,
		0,
		this->pBitMap->GetSize().width,
		this->pBitMap->GetSize().height);
}

