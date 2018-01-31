#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <locale>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs")

class Button
{
public:
	Button();
	~Button();

	const wchar_t* StrToWchar(std::string str);
	void CreateButton(
		ID2D1HwndRenderTarget *renderTarget,
		std::string imageFilePath,
		int left,
		int top,
		int right,
		int bottom);

	ID2D1Bitmap* getBitmapPointer();
	const D2D1_RECT_F getButtonsize() const;
	const D2D1_RECT_F getBitmapRenderSize() const;
	void setButtonsize();
	void setBitmapRendersize();

private:
	IWICFormatConverter *pConverter;
	IWICImagingFactory *pWicFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pBitmapSrc;
	ID2D1Bitmap *pBitMap;
	std::string mFilePath;
	D2D1_RECT_F mButtonSize; // left, top, right, bottom
	D2D1_RECT_F mBitmapRenderSize; // how much of the bitmap the button is gonna show
	

	void CreateWicFactory();
	void LoadImageToBitmap(
		ID2D1HwndRenderTarget *renderTarget, 
		std::string imageFilePath);
};