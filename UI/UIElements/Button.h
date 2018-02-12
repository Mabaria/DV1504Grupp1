#pragma once
#include <locale>
#include "../../GraphicsEngine/DX/Direct2D.h"
#include "../../IO/ObserverPattern/Observable.h"
enum BUTTON_STATE
{
	IDLE,
	HOVER,
	CLICKED,
	RESET
};

class Button : public Observable<Button*>
{
public:
	Button(
		Direct2D *D2D1Panel,
		std::string imageFilePath,
		int left,
		int top,
		int right,
		int bottom);
	~Button();

	const std::wstring StrToWstr(std::string str);
	void CreateButton(
		std::string imageFilePath,
		int left,
		int top,
		int right,
		int bottom);
	void DrawButton();
	void DrawRect();

	ID2D1Bitmap* getBitmapPointer();
	const D2D1_RECT_F GetButtonSize() const;
	const D2D1_RECT_F GetBitmapRenderSize() const;
	const D2D1_RECT_F GetBoundingBoxPercentage() const;
	void SetButtonSize(int left, int top, int right, int bottom);
	void SetBitmapRenderSize(int left, int top, int right, int bottom);
	void SetIconSize(int left, int top, int right, int bottom);
	void MoveButton(int x, int y);
	void MoveIcon(int x, int y);
	void SetButtonStatus(BUTTON_STATE buttState);
	void SetRectStatus(BUTTON_STATE rectState);
	void LoadImageToBitmap(std::string imageFilePath);
	BUTTON_STATE GetButtState() const;

	void SetOpacity(float opacity);
private:
	
	Direct2D *D2D1Panel;
	ID2D1Bitmap *mpBitMap;
	std::string mFilePath;
	D2D1_RECT_F mButtonSize; // left, top, right, bottom

	D2D1_RECT_F mIconSize;

	// how much of the bitmap the button is gonna show!
	D2D1_RECT_F mBitmapRenderSize;
	std::wstring mFilePathAsWstr;
	float mWidth; // Width of bitmap divided by 3
	D2D1_RECT_F mBoundingBoxPercentage;
	float mOpacity;
	bool mBmpLoaded;
	ID2D1SolidColorBrush* mpFailBrush;
	ID2D1SolidColorBrush* mpRectBrush;
	BUTTON_STATE mCurrState;
	void ReleaseCOM(IUnknown *object);
	void mUpdateBoundingBox();
};