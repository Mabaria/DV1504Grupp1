#include "Panel.h"

Panel::Panel(int width, int height, int top, int left, HWND handle, LPCTSTR title)
{
	this->mWidth		= width;
	this->mHeight		= height;
	this->mTop			= top;
	this->mLeft			= left;
	this->mParentWindow	= handle;
	this->mTitle		= title;

	// Creating a child window that will be the canvas to draw on for the panel.
	this->mPanelWindow = CreateWindowEx(
		0,
		this->mTitle,
		this->mTitle,
		WS_CHILD | WS_BORDER | WS_CLIPCHILDREN,
		this->mLeft,
		this->mTop,
		this->mWidth,
		this->mHeight,
		this->mParentWindow,
		0,
		GetModuleHandle(0),
		0);
	ShowWindow(this->mPanelWindow, SW_NORMAL);

	RECT parent_size;
	GetClientRect(this->mParentWindow, &parent_size);
	
	this->mParentHeight = parent_size.bottom - parent_size.top;
	this->mParentWidth	= parent_size.right - parent_size.left;
	this->mParentTop	= parent_size.top;
	this->mParentLeft	= parent_size.left;


}

Panel::~Panel()
{
	for (unsigned int i = 0; i < this->mBitmapVector.size(); i++)
	{
		if (this->mBitmapVector[i].bitmap)
		{
			this->mBitmapVector[i].bitmap->Release();
			this->mBitmapVector[i].bitmap = nullptr;
		}
	}

}

const void Panel::SetWidth(int width)
{
	this->mWidth = width;
}

const int Panel::GetWidth() const
{
	return this->mWidth;
}

const void Panel::SetHeight(int height)
{
	this->mHeight = height;
}

const int Panel::GetHeight() const
{
	return this->mHeight;
}

const void Panel::SetTop(int top)
{
	this->mTop = top;
}

const int Panel::GetTop() const
{
	return this->mTop;
}

const void Panel::SetLeft(int left)
{
	this->mLeft = left;
}

const int Panel::GetLeft() const
{
	return this->mLeft;
}

const void Panel::ScaleX(int scale)
{
	this->mLeft = scale * this->mLeft;
}

const void Panel::ScaleY(int scale)
{
	this->mTop = scale * this->mTop;
}

const bool Panel::Intersects(int x, int y)
{
	// Assumes intersection.
	bool result = true;

	// If it doesn't intersect in either direction it doesn't intersect.
	// Two statements for readability.
	if (x < this->mLeft || x > (this->mLeft + this->mWidth))
	{
		result = false;
	}
	else if (y < this->mTop || y > (this->mTop + this->mHeight))
	{
		result = false;
	}
	return result;
}

const Fraction Panel::IntersectionFraction(int x, int y)
{
	// Returns the fraction as -1, -1 in case 
	// the click does not intersect the panel.
	Fraction fraction;
	fraction.x = -1.0f;
	fraction.y = -1.0f;

	if (Intersects(x, y))
	{
		fraction.x = ((float)x - (float)this->mLeft) / (float)this->mWidth;
		fraction.y = ((float)y - (float)this->mTop) / (float)this->mHeight;
	}
	return fraction;
}

const bool Panel::UpdateWindowSize()
{
	bool updated = false;

	// Getting the client (parent) size.
	RECT client_size;
	GetClientRect(this->mParentWindow, &client_size);
	int client_height	= client_size.bottom - client_size.top;
	int client_width	= client_size.right - client_size.left;

	// Change the panel size if the parent has changed.
	if ((this->mParentWidth != client_width || 
		this->mParentHeight != client_height) 
		&& client_height != 0 
		&& client_width != 0)
	{
		updated = true;
		// Getting how much the client changed in width and height.
		float height_difference_factor = 
			(float)client_height / (float)this->mParentHeight;		
		float width_difference_factor = 
			(float)client_width / (float)this->mParentWidth;

		// Applying these fractions to the member sizes and positions.
		this->mHeight	= (int)round(this->mHeight * height_difference_factor);
		this->mTop		= (int)round(this->mTop	* height_difference_factor);
		this->mWidth	= (int)round(this->mWidth * width_difference_factor);
		this->mLeft		= (int)round(this->mLeft * width_difference_factor);

		// Updating the parent size members.
		this->mParentHeight = client_height;
		this->mParentWidth	= client_width;
		this->mParentTop	= client_size.top;
		this->mParentLeft	= client_size.left;

		// Setting the new position and size to the panel window.
		SetWindowPos(
			this->mPanelWindow, 
			HWND_TOP, // Z order, ignored by SWP_NOZORDER. 
			this->mLeft, 
			this->mTop, 
			this->mWidth, 
			this->mHeight,
			SWP_NOZORDER);
	}
	return updated;
}

void Panel::UpdateWindowPos()
{
	MoveWindow(this->mPanelWindow,
		this->mLeft,
		this->mTop,
		this->mWidth,
		this->mHeight,
		false);
}

bool Panel::IsMouseInsidePanel()
{
	if (Mouse::IsButtonDown(Buttons::Left))
		int i = 0;
	RECT window_rect;
	GetWindowRect(this->mPanelWindow, &window_rect);
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	return PtInRect(&window_rect, mouse_pos); // if mouse is inside panel

}

bool Panel::IsVisible()
{
	return IsWindowVisible(this->mPanelWindow);
}

void Panel::Hide()
{
	MoveWindow(this->mPanelWindow, 0, 0, 0, 0, FALSE);
}

void Panel::Show()
{
	ShowWindow(this->mPanelWindow, SW_NORMAL);
}

HWND *Panel::GetPanelWindowHandle()
{
	return &this->mPanelWindow;
}




