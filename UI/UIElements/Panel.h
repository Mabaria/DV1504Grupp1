#pragma once

#include <Windows.h>

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

	const void ScaleX(int scale);
	const void ScaleY(int scale);

protected:
	int mWidth;
	int mHeight;
	int mTop;
	int mLeft;
	HWND mHandle;

private:
};