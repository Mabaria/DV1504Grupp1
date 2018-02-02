#include "Button.h"

Button::Button(
	Direct2D& D2D1Panel,
	std::string imageFilePath,
	int left,
	int top,
	int right,
	int bottom)
{
	this->mpBitMap = nullptr;
	this->mFilePath = "";
	this->mWidth = 0.0f;
	this->mBoundingBoxPercentage.left = 0.0f;
	this->mBoundingBoxPercentage.right = 0.0f;
	this->mBoundingBoxPercentage.top = 0.0f;
	this->mBoundingBoxPercentage.bottom = 0.0f;
	this->mOpacity = 1.0f;
	this->mBmpLoaded = false;
	this->mpFailBrush = nullptr;
	this->D2D1Panel = &D2D1Panel;
	this->mCurrState = BUTTON_STATE::RESET;
	
	this->CreateButton(
		*this->D2D1Panel, 
		imageFilePath, 
		left, 
		top, 
		right, 
		bottom);

}

Button::~Button()
{
}

const std::wstring Button::StrToWstr(std::string str)
{
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > convert;
	std::wstring result = convert.from_bytes(str);
	this->mFilePathAsWstr = result.c_str();
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
	this->mButtonSize = D2D1::RectF(left, top, right, bottom);
	this->LoadImageToBitmap(D2D1Panel, imageFilePath);
	if (this->mBmpLoaded)
	{	
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mWidth = this->mpBitMap->GetSize().width / 3;
		this->mBoundingBoxPercentage.right = (long)this->mButtonSize.right /
			D2D1Panel.GetpRenderTarget()->GetSize().width;
		this->mBoundingBoxPercentage.top = this->mButtonSize.top /
			D2D1Panel.GetpRenderTarget()->GetSize().height;
		this->mBoundingBoxPercentage.left = this->mButtonSize.left /
			D2D1Panel.GetpRenderTarget()->GetSize().width;
		this->mBoundingBoxPercentage.bottom = this->mButtonSize.bottom /
			D2D1Panel.GetpRenderTarget()->GetSize().height;
	}
	else
	{
		D2D1Panel.GetpRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&mpFailBrush);
	}
}

void Button::DrawButton()
{
	if (this->mBmpLoaded)
	{
		this->D2D1Panel->GetpRenderTarget()->DrawBitmap(
			this->mpBitMap,
			this->mButtonSize,
			this->mOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			this->mBitmapRenderSize);
	}
	else
	{
		this->D2D1Panel->GetpRenderTarget()->FillRectangle(
			this->mButtonSize,
			mpFailBrush
		);
	}
}

ID2D1Bitmap* Button::getBitmapPointer()
{
	return this->mpBitMap;
}

const D2D1_RECT_F Button::GetButtonsize() const
{
	return this->mButtonSize;
}

const D2D1_RECT_F Button::GetBitmapRenderSize() const
{
	return this->mBitmapRenderSize;
}

const D2D1_RECT_F Button::GetBoundingBoxPercentage() const
{
	return this->mBoundingBoxPercentage;
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
	if (!(this->mCurrState == buttState))
	{
		this->mCurrState = buttState;
		if (this->mBmpLoaded)
		{
			this->mBitmapRenderSize = D2D1::RectF(
				this->mWidth * buttState,
				0,
				this->mWidth* (buttState + 1),
				this->mpBitMap->GetSize().height);
			if (buttState == 2)
			{
				this->NotifyObservers(this);
			}
		}
	}
}

void Button::LoadImageToBitmap(
	Direct2D D2D1Panel,
	std::string imageFilePath)
{
	this->mFilePath = imageFilePath;
	this->StrToWstr(imageFilePath);
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
	if (D2D1Panel.GetpBitmapDecoder() != nullptr)
	{
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
			&this->mpBitMap);
		D2D1Panel.GetpFormatConverter()->Release();
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mBmpLoaded = true;
	}
}

BUTTON_STATE Button::GetButtState() const
{
	return this->mCurrState;
}

