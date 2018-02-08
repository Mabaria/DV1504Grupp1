#include "Button.h"

Button::Button(
	Direct2D *D2D1Panel,
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
	this->D2D1Panel = D2D1Panel;
	this->mCurrState = BUTTON_STATE::RESET;
	
	this->CreateButton(
		imageFilePath, 
		left, 
		top, 
		right, 
		bottom);

}

Button::~Button()
{
	this->ReleaseCOM(this->mpBitMap);
	this->ReleaseCOM(this->mpFailBrush);
}

const std::wstring Button::StrToWstr(std::string str)
{ 
	//maybe in tools
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> convert;
	std::wstring result = convert.from_bytes(str);
	this->mFilePathAsWstr = result.c_str();
	return result;
}

void Button::CreateButton(
	std::string imageFilePath, 
	int left, 
	int top, 
	int right, 
	int bottom)
{
	this->mButtonSize = D2D1::RectF(left, top, right, bottom);
	this->LoadImageToBitmap(imageFilePath);
	if (this->mBmpLoaded)
	{	
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mWidth = this->mpBitMap->GetSize().width / 3;
		this->mBoundingBoxPercentage.right = (long)this->mButtonSize.right /
			this->D2D1Panel->GetpRenderTarget()->GetSize().width;
		this->mBoundingBoxPercentage.top = this->mButtonSize.top /
			this->D2D1Panel->GetpRenderTarget()->GetSize().height;
		this->mBoundingBoxPercentage.left = this->mButtonSize.left /
			this->D2D1Panel->GetpRenderTarget()->GetSize().width;
		this->mBoundingBoxPercentage.bottom = this->mButtonSize.bottom /
			this->D2D1Panel->GetpRenderTarget()->GetSize().height;
		this->mUpdateBoundingBox();
	}
	else
	{
		this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
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
		this->mUpdateBoundingBox();
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
	std::string imageFilePath)
{
	this->mFilePath = imageFilePath;
	this->StrToWstr(imageFilePath);
	const wchar_t *p_file_path_wchar = this->mFilePathAsWstr.c_str();
	IWICFormatConverter *converter = this->D2D1Panel->GetpFormatConverter();
	IWICBitmapDecoder *decoder = this->D2D1Panel->GetpBitmapDecoder();
	IWICBitmapFrameDecode *bitmapSrc = this->D2D1Panel->GetpBitmapSrc();

	this->D2D1Panel->GetpImagingFactory()->CreateFormatConverter(&converter);
	this->D2D1Panel->SetpFormatConverter(converter);
	this->D2D1Panel->GetpImagingFactory()->CreateDecoderFromFilename(
		p_file_path_wchar,
		NULL, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnDemand,
		&decoder);
	this->D2D1Panel->SetpBitmapDecoder(decoder);
	if (this->D2D1Panel->GetpBitmapDecoder() != nullptr)
	{			 
		this->D2D1Panel->GetpBitmapDecoder()->GetFrame(0, &bitmapSrc);
		this->D2D1Panel->SetpBitmapSrc(bitmapSrc);
		this->D2D1Panel->GetpFormatConverter()->Initialize(
			bitmapSrc,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut);
		this->D2D1Panel->GetpRenderTarget()->CreateBitmapFromWicBitmap(
			this->D2D1Panel->GetpFormatConverter(),
			NULL,
			&this->mpBitMap);
		this->D2D1Panel->GetpFormatConverter()->Release();
		
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mBmpLoaded = true;
		this->D2D1Panel->GetpBitmapSrc()->Release();
		this->D2D1Panel->GetpBitmapDecoder()->Release();
	}
	
}

BUTTON_STATE Button::GetButtState() const
{
	return this->mCurrState;
}

void Button::ReleaseCOM(IUnknown *object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
}

void Button::mUpdateBoundingBox()
{
	this->mBoundingBoxPercentage.right = (long)this->mButtonSize.right /
		this->D2D1Panel->GetpRenderTarget()->GetSize().width;
	this->mBoundingBoxPercentage.top = this->mButtonSize.top /
		this->D2D1Panel->GetpRenderTarget()->GetSize().height;
	this->mBoundingBoxPercentage.left = this->mButtonSize.left /
		this->D2D1Panel->GetpRenderTarget()->GetSize().width;
	this->mBoundingBoxPercentage.bottom = this->mButtonSize.bottom /
		this->D2D1Panel->GetpRenderTarget()->GetSize().height;
}

