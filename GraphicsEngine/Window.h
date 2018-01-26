#pragma once

#include <string>
#include <Windows.h>
#include "../IO/InputDevice.h"

/*

	-- Window --

	Simple win32 window

	Open() is used for opening window (show window)
	Close() is used for destroying window
	IsOpen() checks if window is open
	Update() does necessary message handling for window to function properly

	Use GetClientSize() to receive size of the drawable surface
	Use GetWindow() to receive HWND

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
	const bool IsOpen() const;
	bool Update();

	const Size& GetClientSize();
	const HWND& GetWindow() const;

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