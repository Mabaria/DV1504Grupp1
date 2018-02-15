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
	this->mFontName = L"Times new roman";
	
	this->mFontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	this->mCreateColor();

	this->SetColor(D2D1::ColorF::Black);
	this->SetText(this->mTextString);

	this->mCreateTextFormat();
}

TextBox::~TextBox()
{
	this->ReleaseCOM(this->mpColor);
	this->ReleaseCOM(this->mpTextFormat);
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
}

void TextBox::DrawTextBox()
{
	this->D2D1Panel->GetpRenderTarget()->DrawTextW(
		this->mpTextWchar,
		wcslen(this->mpTextWchar),
		this->mpTextFormat,
		this->mLayoutRect,
		this->mpColor,
		D2D1_DRAW_TEXT_OPTIONS_CLIP
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
}

void TextBox::SetFontSize(unsigned int size)
{
	this->mFontSize = size;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();
}

void TextBox::SetFontWeight(DWRITE_FONT_WEIGHT fontWeight)
{
	this->mFontWeight = fontWeight;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();
}

void TextBox::SetFontName(std::wstring fontName)
{
	this->mFontName = fontName;
	this->mpTextFormat->Release();
	this->mCreateTextFormat();
}

void TextBox::SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment)
{
	this->mpTextFormat->SetTextAlignment(textAlignment);
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
