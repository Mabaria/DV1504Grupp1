#include "TextBox.h"

TextBox::TextBox(
	Direct2D *D2D1Panel,
	int left,
	int top,
	int right,
	int bottom)
{
	this->D2D1Panel = D2D1Panel;
	this->SetTextBoxSize(left, top, right, bottom);
	this->mTextString = "Default Text ;) Error x00074352EF";
	this->mTextWString = L"";
	this->mpTextWchar = nullptr;
	this->mFontSize = 12;
	std::wstring mTextWString;
	std::string mTextString;
	this->mpTextFormat = nullptr;
	this->mFontName = L"calibri";
	
	this->mFontWeight = DWRITE_FONT_WEIGHT_BOLD;
	this->mCreateColor();

	this->SetColor(D2D1::ColorF::Black);
	this->SetText(this->mTextString);

	this->mCreateTextFormat();
	this->mCreateTextLayout();
}

TextBox::~TextBox()
{
	this->ReleaseCOM(this->mpColor);
	this->ReleaseCOM(this->mpTextFormat);
	this->ReleaseCOM(this->mpTextLayout);
}

D2D1_RECT_F TextBox::GetTextBoxSize() const
{
	return this->mLayoutRect;
}

void TextBox::SetTextBoxSize(int left, int top, int right, int bottom)
{
	this->mLayoutRect = D2D1::RectF(
		(float)left,
		(float)top,
		(float)right,
		(float)bottom);

}

void TextBox::SetText(std::string text)
{
	this->mTextString = text;
	this->mTextWString = this->mStrConverter.from_bytes(text);
	this->mpTextWchar = this->mTextWString.c_str();

	if (this->mpTextLayout)
	{
		this->mpTextLayout->Release();
		this->mCreateTextLayout();
	}
}

void TextBox::DrawTextBox()
{
	/*this->D2D1Panel->GetpRenderTarget()->DrawTextW(
		this->mpTextWchar,
		wcslen(this->mpTextWchar),
		this->mpTextFormat,
		this->mLayoutRect,
		this->mpColor,
		D2D1_DRAW_TEXT_OPTIONS_CLIP
	);*/
	this->mpTextLayout->Draw(
		NULL,
		this->D2D1Panel->GetpTextRenderer(),
		this->mLayoutRect.left, //todo CHECK SO THIS IS CORRECT
		this->mLayoutRect.top   //todo CHECK SO THIS IS CORRECT
	);
}

void TextBox::SetColor(D2D1::ColorF color)
{
	this->mpColor->SetColor(&color);
}

void TextBox::MoveTextBox(int x, int y)
{
	this->mLayoutRect = D2D1::RectF(
		this->mLayoutRect.left + x,
		this->mLayoutRect.top + y,
		this->mLayoutRect.right + x,
		this->mLayoutRect.bottom + y);

	this->mpTextLayout->Release();
	this->mCreateTextLayout();
}

void TextBox::SetFontSize(unsigned int size)
{
	this->mFontSize = size;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();

	this->mpTextLayout->Release();
	this->mCreateTextLayout();
}

void TextBox::SetFontWeight(DWRITE_FONT_WEIGHT fontWeight)
{
	this->mFontWeight = fontWeight;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();

	this->mpTextLayout->Release();
	this->mCreateTextLayout();
}

void TextBox::SetFontName(std::wstring fontName)
{
	this->mFontName = fontName;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();

	this->mpTextLayout->Release();
	this->mCreateTextLayout();
}

void TextBox::SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment)
{
	this->mpTextFormat->SetTextAlignment(textAlignment);

	this->mpTextLayout->Release();
	this->mCreateTextLayout();
}

void TextBox::mCreateTextLayout()
{
	HRESULT hr = this->D2D1Panel->GetpTextFactory()->CreateTextLayout(
		this->mpTextWchar,									// string
		wcslen(this->mpTextWchar),							// string length
		this->mpTextFormat,									// text format
		this->mLayoutRect.right - this->mLayoutRect.left,	// max width
		this->mLayoutRect.bottom - this->mLayoutRect.top,	// max height
		&this->mpTextLayout									// return pointer
	);

	DWRITE_TEXT_RANGE text_range = { 0, wcslen(this->mpTextWchar)};
	IDWriteFontCollection* pFontCollection = nullptr;
	// Get the system font collection
	hr = this->D2D1Panel->GetpTextFactory()->GetSystemFontCollection(&pFontCollection);
	// Set the specified font
	this->mpTextLayout->SetFontCollection(pFontCollection, text_range);
	this->mpTextLayout->SetFontFamilyName(this->mFontName.c_str(), text_range);
	pFontCollection->Release();
}

void TextBox::mCreateColor()
{
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&this->mpColor
	);
}

void TextBox::mCreateTextFormat()
{

	HRESULT hr = this->D2D1Panel->GetpTextFactory()->CreateTextFormat(
		this->mFontName.c_str(),
		NULL,
		this->mFontWeight,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)this->mFontSize,
		L"sv-SE",
		&this->mpTextFormat
	);
	this->mpTextFormat->SetTrimming(&this->D2D1Panel->GetTrimmer(), NULL);
	this->mpTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);
}

void TextBox::ReleaseCOM(IUnknown * object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
}
