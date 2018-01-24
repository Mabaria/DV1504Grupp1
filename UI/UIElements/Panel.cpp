#include "Panel.h"

Panel::Panel(int width, int height, int top, int left, HWND handle)
{
	this->mWidth  = width;
	this->mHeight = height;
	this->mTop	  = top;
	this->mLeft	  = left;
	this->mHandle = handle;
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
	else if (y < this->mTop || y > (this->mTop + this->mHeight));
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
