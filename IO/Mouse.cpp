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

const float Mouse::GetMouseX()
{
	return InputDevice::mousePosition.x;
}

const float Mouse::GetMouseY()
{
	return InputDevice::mousePosition.y;
}

const Position Mouse::GetMousePosition()
{
	return InputDevice::mousePosition;
}

const float Mouse::GetMouseXPercentage()
{
	return InputDevice::mousePositionPercentage.x;
}

const float Mouse::GetMouseYPercentage()
{
	return InputDevice::mousePositionPercentage.y;
}

const Position Mouse::GetMousePositionPercentage()
{
	return InputDevice::mousePositionPercentage;
}

