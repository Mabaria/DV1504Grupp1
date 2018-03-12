#pragma once
#include <locale>
#include "../../GraphicsEngine/DX/Direct2D.h"
#include "../../IO/ObserverPattern/Observable.h"
enum BUTTON_STATE
{
	IDLE,
	HOVER,
	CLICKED,
	RESET,
	RELEASED
};

class Button : public Observable<Button>
{
public:
	Button(
		Direct2D *D2D1Panel,
		std::string imageFilePath,
		int left,
		int top,
		int right,
		int bottom,
		std::string name = "");
	Button(
		Direct2D *D2D1Panel,
		ID2D1Bitmap* bitmapPointer,
		int left,
		int top,
		int right,
		int bottom,
		std::string name = "");
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
	void DrawFilledRect(float r, float g, float b, float a = 1.f);
	void DrawFilledRect();
	

	ID2D1Bitmap* getBitmapPointer();
	const D2D1_RECT_F GetButtonSize() const;
	const D2D1_RECT_F GetBitmapRenderSize() const;
	const D2D1_RECT_F GetBoundingBoxPercentage() const;
	void SetButtonSize(int left, int top, int right, int bottom);
	void SetBitmapRenderSize(int left, int top, int right, int bottom);
	void SetIconSize(int left, int top, int right, int bottom);
	void MoveButton(int x, int y);
	void MoveIcon(int x, int y);
	bool SetButtonStatus(BUTTON_STATE buttState);
	void SetRectStatus(BUTTON_STATE rectState);
	void SetBitmap(ID2D1Bitmap* bitmapPointer);
	void LoadImageToBitmap(std::string imageFilePath);
	void SetRenderWidth(float width);
	BUTTON_STATE GetButtState() const;

	void SetOpacity(float opacity);
	std::string GetName() const;
	void SetRectColor(float r, float g, float b, float a = 1.f);
	void SetButtonColor(float r, float g, float b, float a = 1.f);
	const D2D1_COLOR_F GetButtonColor() const;
	const float GetOpacity();
	void SetAlive(bool alive);
	const bool GetAlive();

	void ForceButtState(BUTTON_STATE newState); /* Forces the button to stay in the
												selected state until ForcedButtState
												is set to false or ToggleForcedButtState
												is called */
	bool ToggleForcedButtState();
	void SetForcedButtState(bool newForcedState);/*Set this to false if you want to
												 unlock a currently forced button.
												 Set to true if you wish to lock the
												 button in it's current state */

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
	ID2D1SolidColorBrush* mpFillBrush;
	BUTTON_STATE mCurrState;
	void ReleaseCOM(IUnknown *object);
	void mUpdateBoundingBox();

	bool mBitmapLoadedByFilePath;
	std::string mName;

	bool mAlive;
	bool mForcedButtState = false;	
};