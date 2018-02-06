#pragma once

#include <Windows.h>

struct Fraction
{
	float x;
	float y;
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

	const void UpdateWindowSize();

protected:
	int mWidth;
	int mHeight;
	int mTop;
	int mLeft;

	// Normalized size relative to the parent window.
	float mWidthFraction;
	float mHeightFraction;
	float mTopFraction;
	float mLeftFraction;

	// To compare sizes.
	HWND mParentWindow;
	HWND mPanelWindow;

private:
};