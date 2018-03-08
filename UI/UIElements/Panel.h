#pragma once

#include <Windows.h>
#include <math.h>
#include "../../IO/Mouse.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include <locale>

struct Fraction
{
	float x;
	float y;
};
struct BitmapInfo
{
	ID2D1Bitmap* bitmap = nullptr;
	std::string name = "";
};

class Panel
{
public:
	Panel(int width, int height, int top, int left, HWND handle, LPCTSTR title);
	~Panel();

	const void SetWidth(int width);
	const int GetWidth() const;

	const void SetHeight(int height);
	const int GetHeight() const;

	const void SetTop(int top);
	const int GetTop() const;

	const void SetLeft(int left);
	const int GetLeft() const;

	// Scales the panel by the parameter in chosen direction.
	const void ScaleX(int scale);
	const void ScaleY(int scale);

	// Returns whether or not the parameters intersect the panel.
	// Assumes click coordinates in pixels starting from the top left
	// corner of the main window.
	const bool Intersects(int x, int y);

	// Returns the position of the click with regard 
	// to the panel's borders in range [0;1].
	const Fraction IntersectionFraction(int x, int y);

	const bool UpdateWindowSize();
	void UpdateWindowPos();

	bool IsMouseInsidePanel();

	bool IsVisible();
	void Hide();
	void Show();
	
	// Does not work, do not use.
	void ShowOnTop();

	HWND *GetPanelWindowHandle();

	void LoadImageToBitmap(std::string imageFilePath, std::string bitmapName);
	ID2D1Bitmap* GetBitmapByName(std::string bitmapName);

	Direct2D* GetDirect2D();

protected:
	Direct2D * mDirect2D;
	int mWidth;
	int mHeight;
	int mTop;
	int mLeft;

	int mParentWidth;
	int mParentHeight;
	int mParentTop;
	int mParentLeft;

	bool mIsVisible;

	// To compare sizes.
	HWND mParentWindow;
	HWND mPanelWindow;
	LPCTSTR mTitle;

	std::vector<BitmapInfo> mBitmapVector;


private:
};