#include "Panel.h"

Panel::Panel(int width, int height, int top, int left, HWND handle, LPCTSTR title)
{
	this->mWidth		= width;
	this->mHeight		= height;
	this->mTop			= top;
	this->mLeft			= left;
	this->mParentWindow	= handle;

	// Creating a child window that will be the canvas to draw on for the panel.
	this->mPanelWindow = CreateWindowEx(
		0,
		title,
		title,
		WS_CHILD | WS_BORDER,
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
		
	int parent_height = parent_size.bottom - parent_size.top;
	int parent_width = parent_size.right - parent_size.left;

	this->mHeightFraction = (float)this->mHeight / (float)parent_height;
	this->mWidthFraction = (float)this->mWidth / (float)parent_width;
	this->mTopFraction = (float)this->mTop / (float)parent_height;
	this->mLeftFraction = (float)this->mLeft / (float)parent_width;	
}

Panel::~Panel()
{
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

const void Panel::UpdateWindowSize()
{
	RECT client_size;
	GetClientRect(this->mParentWindow, &client_size);
	int client_height	= client_size.bottom - client_size.top;
	int client_width	= client_size.right - client_size.left;
	if ((this->mParentWidth != client_width || 
		this->mParentHeight != client_height) 
		&& client_height != 0 
		&& client_width != 0)
	{
		float height_difference_factor = client_height / this->mParentHeight;
		float width_difference_factor = client_width / this->mParentWidth;

		this->mHeight *= height_difference_factor;
		this->mTop *= height_difference_factor;
		this->mWidth *= width_difference_factor;
		this->mLeft *= width_difference_factor;

		this->mParentHeight = client_height;
		this->mParentWidth = client_width;
		this->mParentTop = client_size.top;
		this->mParentLeft = client_size.left;

		RECT panel_size = { this->mLeft,
			this->mTop,
			this->mLeft + this->mWidth,
			this->mTop + this->mHeight };

		AdjustWindowRectEx(&panel_size, WS_CHILD | WS_BORDER, false, 0);
	}
}
