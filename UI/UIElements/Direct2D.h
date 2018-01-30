#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <vector>
#include <Windows.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs")

class Direct2D
{
public:
	Direct2D();
	~Direct2D();
	void CreateRenderTarget(HWND window, unsigned int width, unsigned int height);
	void LoadImageToBitmap(LPCWSTR imageFilePath);
	

private:
	struct ImgStruct
	{
		ID2D1Bitmap *pBitMap;
		std::string FilePath;
	};
	ID2D1HwndRenderTarget *pRenderTarget;
	//std::vector<ID2D1Bitmap*> pBitMaps;
	std::vector<ImgStruct> mImages;
	IWICFormatConverter *pConverter;
	ID2D1Factory *pFactory;
	IWICImagingFactory *pWicFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pBitmapSrc;
	
	
	void CreateFactory();
	void CreateWicFactory();
	void Init();
};