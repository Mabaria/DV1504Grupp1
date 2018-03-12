#include "../GraphicsEngine/Window.h"
#include <iostream>

// Necessary for window
LRESULT CALLBACK WindowProcedure(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	POINT cursor_pos = { 0 };
	RECT client_rect = { 0 };
	RECT window_rect = { 0 };

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{

		case VK_ESCAPE:
			InputDevice::SetKey(Keys::Esc, true);
			break;

		case VK_RETURN:
			InputDevice::SetKey(Keys::Enter, true);
			break;

		case 0x57:
			InputDevice::SetKey(Keys::W, true);
			break;

		case 0x41:
			InputDevice::SetKey(Keys::A, true);
			break;

		case 0x53:
			InputDevice::SetKey(Keys::S, true);
			break;

		case 0x44:
			InputDevice::SetKey(Keys::D, true);
			break;

		case 0x51:
			InputDevice::SetKey(Keys::Q, true);
			break;

		case 0x45:
			InputDevice::SetKey(Keys::E, true);
			break;

		case 16:
			InputDevice::SetKey(Keys::Shift, true);
			break;

		case 17:
			InputDevice::SetKey(Keys::Ctrl, true);
			break;

		case 0x20:
			InputDevice::SetKey(Keys::Space, true);
			break;

		case VK_TAB:
			InputDevice::SetKey(Keys::Tab, true);
			break;

		case VK_BACK:
			InputDevice::SetKey(Keys::Backspace, true);
			break;

		case 49:
			InputDevice::SetKey(Keys::One, true);
			break;

		case 50:
			InputDevice::SetKey(Keys::Two, true);
			break;

		case 51:
			InputDevice::SetKey(Keys::Three, true);
			break;

		case 52:
			InputDevice::SetKey(Keys::Four, true);
			break;

		}

		break;
	}

	case WM_KEYUP:
	{
		switch (wParam)
		{

		case VK_ESCAPE:
			InputDevice::SetKey(Keys::Esc, false);
			break;

		case VK_RETURN:
			InputDevice::SetKey(Keys::Enter, false);
			break;

		case 0x57:
			InputDevice::SetKey(Keys::W, false);
			break;

		case 0x41:
			InputDevice::SetKey(Keys::A, false);
			break;

		case 0x53:
			InputDevice::SetKey(Keys::S, false);
			break;

		case 0x44:
			InputDevice::SetKey(Keys::D, false);
			break;

		case 0x51:
			InputDevice::SetKey(Keys::Q, false);
			break;

		case 0x45:
			InputDevice::SetKey(Keys::E, false);
			break;

		case 16:
			InputDevice::SetKey(Keys::Shift, false);
			break;

		case 17:
			InputDevice::SetKey(Keys::Ctrl, false);
			break;

		case 0x20:
			InputDevice::SetKey(Keys::Space, false);
			break;

		case VK_TAB:
			InputDevice::SetKey(Keys::Tab, false);
			break;

		case VK_BACK:
			InputDevice::SetKey(Keys::Backspace, false);
			break;

		case 49:
			InputDevice::SetKey(Keys::One, false);
			break;

		case 50:
			InputDevice::SetKey(Keys::Two, false);
			break;

		case 51:
			InputDevice::SetKey(Keys::Three, false);
			break;

		case 52:
			InputDevice::SetKey(Keys::Four, false);
			break;
		}

		break;
	}

	case WM_MOUSEMOVE:
	{
		GetCursorPos(&cursor_pos);
		GetWindowRect(hWnd, &window_rect);
		GetClientRect(hWnd, &client_rect);

		InputDevice::SetExactMousePosition(
			cursor_pos.x,
			cursor_pos.y
		);

		InputDevice::SetExactMousePositionPercentage(
			(float)cursor_pos.x / (float)client_rect.right,
			(float)cursor_pos.y / (float)client_rect.bottom
		);
		// Set a relative position from the client area
		cursor_pos.x -= window_rect.left;
		cursor_pos.y -= window_rect.top;
		InputDevice::SetMousePosition(
			cursor_pos.x,
			cursor_pos.y
		);

		// Set a percentage across the client screen
		InputDevice::SetMousePositionPercentage(
			(float)cursor_pos.x / (float)client_rect.right,
			(float)cursor_pos.y / (float)client_rect.bottom
		);
		break;
	}

	case WM_LBUTTONDOWN:
		InputDevice::SetMouseButton(Buttons::Left, true);
		break;

	case WM_LBUTTONUP:
		InputDevice::SetMouseButton(Buttons::Left, false);
		break;

	case WM_RBUTTONDOWN:
		InputDevice::SetMouseButton(Buttons::Right, true);
		break;

	case WM_RBUTTONUP:
		InputDevice::SetMouseButton(Buttons::Right, false);
		break;

	case WM_MOUSEWHEEL:
		InputDevice::SetMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f);
		break;

	case 519:
		InputDevice::SetMouseButton(Buttons::ScrollPress, true);
		break;

	case 520:
		InputDevice::SetMouseButton(Buttons::ScrollPress, false);
		break;

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
		AdjustWindowRectEx(&window_size, 0, false, 0);

		this->mHwnd = CreateWindowEx(
			0,
			this->mTitle.c_str(),
			this->mTitle.c_str(),
			WS_POPUP | WS_CLIPCHILDREN,
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

void Window::OpenNormal()
{
	if (this->mHwnd && !this->mIsOpen)
	{
		ShowWindow(this->mHwnd, SW_SHOWNORMAL);
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
		//DestroyWindow(this->mHwnd);
	}
}

bool Window::Update()
{
	InputDevice::SetMouseScroll(0);
	// Sets previous buttons to last updates buttons
	InputDevice::buttonsPrevious = InputDevice::buttonsActive;
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
