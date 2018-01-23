#pragma once

#include <string>
#include <Windows.h>

struct Size {
	int width, height;
};

class Window
{
public:
	Window(
		const std::wstring title, 
		const int clientWidth, 
		const int clientHeight
	);

	~Window();

	void Open();
	void Close();
	bool IsOpen();
	bool Update();

	const Size& GetClientSize();

private:
	int mClientWidth, mClientHeight;
	bool mIsOpen;

	std::wstring mTitle;
	HWND mHwnd;
	WNDCLASSEX mWndClass;
	MSG mMsg;

	Size mSize;

	void HandleUpdates();
};