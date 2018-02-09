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


private:

	Direct2D *D2D1Panel;
	ID2D1SolidColorBrush *mpColor;
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > 
		mStrConverter;
	std::wstring mTextWString;
	std::string mTextString;
	const wchar_t *mpTextWchar;
	D2D1_RECT_F mLayoutRect;

	void mCreateColor();
	void ReleaseCOM(IUnknown *object);
};
