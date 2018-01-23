#pragma once

#include <string>
#include <Windows.h>

/*

	-- Window --

	Simple win32 window

	Open() is used for opening window (show window)
	Close() is used for destroying window
	IsOpen() checks if window is open
	Update() does necessary message handling for window to function properly

	Use GetClientSize() to receive size of the drawable surface

*/


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