#include "Mouse.h"

const bool Mouse::IsButtonPressed(const Buttons button)
{
	bool flag = false;

	if (InputDevice::buttonsPressed[button]
		&& !InputDevice::buttonsActive[button])
	{
		flag 
			= InputDevice::buttonsActive[button] 
			= InputDevice::buttonsPressed[button];
	}

	return flag;
}

const bool Mouse::IsButtonDown(const Buttons button)
{
	return InputDevice::buttonsPressed[button];
}

const float Mouse::GetX()
{
	return InputDevice::mousePosition.x;
}

const float Mouse::GetY()
{
	return InputDevice::mousePosition.y;
}

const Position Mouse::GetPosition()
{
	return InputDevice::mousePosition;
}

const float Mouse::GetXPercentage()
{
	return InputDevice::mousePositionPercentage.x;
}

const float Mouse::GetYPercentage()
{
	return InputDevice::mousePositionPercentage.y;
}

const Position Mouse::GetPositionPercentage()
{
	return InputDevice::mousePositionPercentage;
}

const float Mouse::GetScroll()
{
	return InputDevice::scrollDirection;
}

