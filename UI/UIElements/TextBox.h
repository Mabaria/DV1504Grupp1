#pragma once
#include <locale>
#include "../../GraphicsEngine/DX/Direct2D.h"

class TextBox
{
public:
	TextBox(
		Direct2D *D2D1Panel,
		int left,
		int top,
		int right,
		int bottom);
	~TextBox();

	D2D1_RECT_F GetTextBoxSize() const;
	void SetTextBoxSize(int left, int top, int right, int bottom);
	void SetText(std::string text);
	void DrawTextBox();
	void SetColor(D2D1::ColorF color);
	void MoveTextBox(int x, int y);
	void SetFontSize(unsigned int size);
	void SetFontWeight(DWRITE_FONT_WEIGHT fontWeight);
	void SetFontName(std::wstring fontName);
	void SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment);


private:

	Direct2D *D2D1Panel;
	IDWriteTextFormat *mpTextFormat;
	ID2D1SolidColorBrush *mpColor;
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > 
		mStrConverter;
	std::wstring mTextWString;
	std::string mTextString;
	const wchar_t *mpTextWchar;
	D2D1_RECT_F mLayoutRect;
	unsigned int mFontSize;
	DWRITE_FONT_WEIGHT mFontWeight;
	std::wstring mFontName;
	DWRITE_TEXT_ALIGNMENT mAlignment;

	void mCreateColor();
	void mCreateTextFormat();
	void ReleaseCOM(IUnknown *object);
};
