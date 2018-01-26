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
	Panel(int width, int height, int top, int left, HWND handle);
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
	const bool Intersects(int x, int y);

	// Returns the position of the click with regard 
	// to the panel's borders in range [0;1].
	const Fraction IntersectionFraction(int x, int y);

protected:
	int mWidth;
	int mHeight;
	int mTop;
	int mLeft;
	HWND mHandle;

private:
};