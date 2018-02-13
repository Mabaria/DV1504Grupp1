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
	std::wstring mTextWString;
	std::string mTextString;
	this->mCreateColor();
	this->SetColor(D2D1::ColorF::Black);
	this->SetText(this->mTextString);
}

TextBox::~TextBox()
{
	this->ReleaseCOM(this->mpColor);
}

D2D1_RECT_F TextBox::GetTextBoxSize() const
{
	return this->mLayoutRect;
}

void TextBox::SetTextBoxSize(int left, int top, int right, int bottom)
{
	this->mLayoutRect = D2D1::RectF(left, top, right, bottom);
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
		this->D2D1Panel->GetpTextFormat(),
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

void TextBox::mCreateColor()
{
	this->D2D1Panel->GetpRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&this->mpColor
	);
}

void TextBox::ReleaseCOM(IUnknown * object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
}
