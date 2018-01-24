#include "../GraphicsEngine/Window.h"

// Necessary for window
LRESULT CALLBACK WindowProcedure(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Create window with desired client size and title
Window::Window(
	const std::wstring title, 
	const int clientWidth, 
	const int clientHeight)
{
	this->mClientWidth = clientWidth;
	this->mClientHeight = clientHeight;
	
	this->mIsOpen = false;
	this->mTitle = title;

	// Set settings for window
	this->mWndClass = {
		sizeof(WNDCLASSEX),
		CS_DBLCLKS,
		WindowProcedure,
		0,
		0,
		GetModuleHandle(0),
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		HBRUSH(COLOR_WINDOW + 1),
		0,
		this->mTitle.c_str(),
		LoadIcon(0, IDI_APPLICATION)
	};

	// If registration succeeded then create window with desired size
	if (RegisterClassEx(&this->mWndClass))
	{
		RECT window_size = { 0, 0, this->mClientWidth, this->mClientHeight };
		AdjustWindowRectEx(&window_size, WS_OVERLAPPEDWINDOW, false, 0);

		this->mHwnd = CreateWindowEx(
			0,
			this->mTitle.c_str(),
			this->mTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			window_size.right - window_size.left,	// Width of window
			window_size.bottom - window_size.top,	// Height of window
			0,
			0,
			GetModuleHandle(0),
			0
		);
	}
}

Window::~Window()
{

}

void Window::Open()
{
	if (this->mHwnd && !this->mIsOpen)
	{
		ShowWindow(this->mHwnd, SW_MAXIMIZE);
		this->mIsOpen = true;
	}
}

const bool Window::IsOpen() const
{
	return this->mIsOpen;
}

void Window::Close()
{
	if (this->mHwnd && this->mIsOpen)
	{
		this->mIsOpen = false;
		DestroyWindow(this->mHwnd);
	}
}

bool Window::Update()
{
	bool was_updated = false; // See if message is handled
	if (PeekMessage(&this->mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&this->mMsg);
		DispatchMessage(&this->mMsg);
		this->HandleUpdates();
		was_updated = true;
	}

	return was_updated;
}

void Window::HandleUpdates()
{
	switch (this->mMsg.message)
	{
	case WM_QUIT:
		this->Close();	// Close window
		break;

	case WM_SIZE:
		break;

	default:
		break;
	}
}

const Size& Window::GetClientSize()
{
	// If window size changed, set new values
	RECT client_size;
	GetClientRect(this->mHwnd, &client_size);
	if (client_size.right != this->mClientWidth ||
		client_size.bottom != this->mClientHeight)
	{
		this->mClientWidth = client_size.right;
		this->mClientHeight = client_size.bottom;
	}

	// Push values into struct
	this->mSize.width  = this->mClientWidth;
	this->mSize.height = this->mClientHeight;

	return this->mSize;
}

const HWND& Window::GetWindow() const
{
	return this->mHwnd;
}
