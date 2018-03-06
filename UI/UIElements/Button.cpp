#include "Button.h"

Button::Button(
	Direct2D *D2D1Panel,
	std::string imageFilePath,
	int left,
	int top,
	int right,
	int bottom,
	std::string name)
{
	this->mName = name;
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
	this->mpRectBrush = nullptr;
	this->mpFillBrush = nullptr;
	this->D2D1Panel = D2D1Panel;
	this->mCurrState = BUTTON_STATE::RESET;
	this->mBitmapLoadedByFilePath = true;
	/*this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(1.f, 0.f, 0.f, 1.0f)),
		&this->mpFillBrush);*/
	
	this->CreateButton(
		imageFilePath, 
		left, 
		top, 
		right, 
		bottom);

}

Button::Button(
	Direct2D * D2D1Panel,
	ID2D1Bitmap * bitmapPointer,
	int left,
	int top,
	int right,
	int bottom,
	std::string name)
{
	this->mpBitMap = bitmapPointer;
	this->mName = name;

	this->mWidth = 0.0f;
	this->mBoundingBoxPercentage.left = 0.0f;
	this->mBoundingBoxPercentage.right = 0.0f;
	this->mBoundingBoxPercentage.top = 0.0f;
	this->mBoundingBoxPercentage.bottom = 0.0f;
	this->mOpacity = 1.0f;
	this->mBmpLoaded = false;
	this->mpFailBrush = nullptr;
	this->mpRectBrush = nullptr;
	this->mpFillBrush = nullptr;
	this->D2D1Panel = D2D1Panel;
	this->mCurrState = BUTTON_STATE::RESET;
	this->mBitmapLoadedByFilePath = false;

	
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.75f, 0.75f, 0.75f, 1.0f)),
		&this->mpRectBrush);
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(1.f, 0.f, 0.f, 1.0f)),
		&this->mpFillBrush);
	this->mButtonSize = D2D1::RectF(
		(float)left,
		(float)top,
		(float)right,
		(float)bottom);
	this->mIconSize = this->mButtonSize;

	if (this->mpBitMap)
	{
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mWidth = this->mpBitMap->GetSize().width / 3;
		this->mUpdateBoundingBox();
		this->mBmpLoaded = true;
	}
	else
	{
		this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&mpFailBrush);
	}
}

Button::~Button()
{
	if (this->mBitmapLoadedByFilePath)
	{
		this->ReleaseCOM(this->mpBitMap);

	}
	this->ReleaseCOM(this->mpFailBrush);
	this->ReleaseCOM(this->mpRectBrush);
	this->ReleaseCOM(this->mpFillBrush);
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
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.75f, 0.75f, 0.75f, 1.0f)),
		&this->mpRectBrush);
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(1.f, 1.f, 1.f, 1.0f)),
		&this->mpFillBrush);
	this->mButtonSize = D2D1::RectF(
		(float)left, 
		(float)top, 
		(float)right, 
		(float)bottom);
	this->mIconSize = this->mButtonSize;
	if (this->mBitmapLoadedByFilePath)
	{
		this->LoadImageToBitmap(imageFilePath);
	}	
	if (this->mBmpLoaded)
	{	
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mWidth = this->mpBitMap->GetSize().width / 3;

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
			this->mIconSize,
			this->mOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			this->mBitmapRenderSize);
		this->mUpdateBoundingBox();
	}
	else
	{
		this->D2D1Panel->GetpRenderTarget()->FillRectangle(
			this->mButtonSize,
			this->mpFillBrush);
	}
}

void Button::DrawRect()
{
	this->D2D1Panel->GetpRenderTarget()->DrawRectangle(
		this->mButtonSize,
		this->mpRectBrush,
		2.0f);
}

void Button::DrawFilledRect(float r, float g, float b, float a)
{
	this->mpFillBrush->SetColor(D2D1::ColorF(r,g,b,a));
	this->D2D1Panel->GetpRenderTarget()->FillRectangle(
		this->mButtonSize,
		this->mpFillBrush);
}

void Button::DrawFilledRect()
{
	this->D2D1Panel->GetpRenderTarget()->FillRectangle(
		this->mButtonSize,
		this->mpFillBrush);
}

ID2D1Bitmap* Button::getBitmapPointer()
{
	return this->mpBitMap;
}

const D2D1_RECT_F Button::GetButtonSize() const
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

void Button::SetButtonSize(int left, int top, int right, int bottom)
{
	this->mButtonSize = D2D1::RectF(
		(float)left, 
		(float)top, 
		(float)right, 
		(float)bottom);
}

void Button::SetBitmapRenderSize(int left, int top, int right, int bottom)
{
	this->mBitmapRenderSize = D2D1::RectF(
		(float)left, 
		(float)top, 
		(float)right, 
		(float)bottom);
}

void Button::SetIconSize(int left, int top, int right, int bottom)
{
	this->mIconSize = D2D1::RectF(
		(float)left,
		(float)top, 
		(float)right, 
		(float)bottom);
}

void Button::MoveButton(int x, int y)
{
	this->mButtonSize = D2D1::RectF(
		this->mButtonSize.left + x,
		this->mButtonSize.top + y,
		this->mButtonSize.right + x,
		this->mButtonSize.bottom + y);
	this->MoveIcon(x, y);
}

void Button::MoveIcon(int x, int y)
{
	this->mIconSize = D2D1::RectF(
		this->mIconSize.left + x,
		this->mIconSize.top + y,
		this->mIconSize.right + x,
		this->mIconSize.bottom + y);
}

void Button::SetButtonStatus(BUTTON_STATE buttState)
{
	if (!(this->mCurrState == buttState))
	{
		this->mCurrState = buttState;
		if (this->mBmpLoaded)
		{
			if (this->mpBitMap->GetSize().width != this->mWidth)
			{
				this->mBitmapRenderSize = D2D1::RectF(
					this->mWidth * buttState,
					0,
					this->mWidth* (buttState + 1),
					this->mpBitMap->GetSize().height);
			}
			if (buttState == 2)
			{
				this->NotifyObservers(this);
			}
		}
	}
}

void Button::SetRectStatus(BUTTON_STATE rectState)
{
	if (!(this->mCurrState == rectState))
	{
		this->mCurrState = rectState;
		switch (rectState)
		{
		case BUTTON_STATE::HOVER:
			this->mpRectBrush->SetColor(D2D1::ColorF(D2D1::ColorF(
				0.50f, 
				0.50f, 
				0.50f, 
				1.0f)));
			break;
		case BUTTON_STATE::CLICKED:
			this->mpRectBrush->SetColor(D2D1::ColorF(D2D1::ColorF(
				0.00f,
				0.00f,
				0.00f,
				1.0f)));
			this->NotifyObservers(this);
			break;
		default:
			this->mpRectBrush->SetColor(D2D1::ColorF(D2D1::ColorF(
				0.75f, 
				0.75f, 
				0.75f, 
				1.0f)));
			break;
		}
	}
}

void Button::SetBitmap(ID2D1Bitmap * bitmapPointer)
{
	if (bitmapPointer)
	{

		if (this->mBitmapLoadedByFilePath)
		{
			this->ReleaseCOM(this->mpBitMap);

		}
		this->mpBitMap = bitmapPointer;
		this->mBitmapLoadedByFilePath = false;
	}
	else
	{
		MessageBoxA(
			NULL, 
			"Class Error: #BUTTON : Bitmap was nullptr", 
			NULL, NULL
		);
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
		this->mBitmapRenderSize = D2D1::RectF(
			0,
			0,
			this->mpBitMap->GetSize().width,
			this->mpBitMap->GetSize().height);
		this->mBmpLoaded = true;
	}	
}

void Button::SetRenderWidth(float width)
{
	this->mWidth = width;
}

BUTTON_STATE Button::GetButtState() const
{
	return this->mCurrState;
}

void Button::SetOpacity(float opacity)
{
	this->mOpacity = opacity;
}

std::string Button::GetName() const
{
	return this->mName;
}

void Button::SetRectColor(float r, float g, float b, float a)
{
	this->mpRectBrush->SetColor(D2D1::ColorF(r, g, b, a));
}

void Button::SetButtonColor(float r, float g, float b, float a)
{
	this->mpFillBrush->SetColor(D2D1::ColorF(r, g, b, a));
}

const D2D1_COLOR_F Button::GetButtonColor() const
{
	return this->mpFillBrush->GetColor();
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
	this->mBoundingBoxPercentage.right = this->mButtonSize.right /
		this->D2D1Panel->GetpRenderTarget()->GetSize().width;
	this->mBoundingBoxPercentage.top = this->mButtonSize.top /
		this->D2D1Panel->GetpRenderTarget()->GetSize().height;
	this->mBoundingBoxPercentage.left = this->mButtonSize.left /
		this->D2D1Panel->GetpRenderTarget()->GetSize().width;
	this->mBoundingBoxPercentage.bottom = this->mButtonSize.bottom /
		this->D2D1Panel->GetpRenderTarget()->GetSize().height;
}

