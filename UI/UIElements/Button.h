#pragma once
#include <locale>
#include "../../GraphicsEngine/DX/Direct2D.h"
enum BUTTON_STATE
{
	IDLE,
	HOVER,
	CLICKED
};


class Button
{
public:
	Button();
	~Button();

	const wchar_t *StrToWchar(std::string str);
	void CreateButton(
		Direct2D D2D1Panel,
		std::string imageFilePath,
		int left,
		int top,
		int right,
		int bottom);
	void DrawButton(Direct2D D2D1Panel);

	ID2D1Bitmap* getBitmapPointer();
	const D2D1_RECT_F GetButtonsize() const;
	const D2D1_RECT_F GetBitmapRenderSize() const;
	void SetButtonsize(int left, int top, int right, int bottom);
	void SetBitmapRendersize(int left, int top, int right, int bottom);
	void SetButtonStatus(BUTTON_STATE buttState);
	void LoadImageToBitmap(
		Direct2D D2D1Panel,
		std::string imageFilePath);

private:

	ID2D1Bitmap *pBitMap;
	std::string mFilePath;
	D2D1_RECT_F mButtonSize; // left, top, right, bottom
	D2D1_RECT_F mBitmapRenderSize; // how much of the bitmap the button is gonna show
	std::wstring mFilePathAsWstr;

	
	
};