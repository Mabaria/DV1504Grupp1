#include "Mouse.h"

const bool Mouse::IsButtonPressed(const Buttons button)
{
	// If active and previous does NOT match with button
	//char step2 = 0;
	//char step1 = (InputDevice::buttonsActive & ~InputDevice::buttonsPrevious);
	//if ((1 << button) & step1)
	//	step2 = (1 << button) & step1;
	return (1 << button) & 
		(InputDevice::buttonsActive & ~InputDevice::buttonsPrevious);
}

const bool Mouse::IsButtonDown(const Buttons button)
{
	return (1 << button) & InputDevice::buttonsActive;
}

const bool Mouse::IsButtonReleased(const Buttons button)
{
	//return false;
	return (1 << button) &
		(InputDevice::buttonsPrevious & ~InputDevice::buttonsActive);
}

const int Mouse::GetX()
{
	return InputDevice::mousePosition.x;
}

const int Mouse::GetY()
{
	return InputDevice::mousePosition.y;
}

const int Mouse::GetExactX()
{
	return InputDevice::exactMousePosition.x;
}

const int Mouse::GetExactY()
{
	return InputDevice::exactMousePosition.y;
}

const Position Mouse::GetPosition()
{
	return InputDevice::mousePosition;
}

const Position Mouse::GetExactPosition()
{
	return InputDevice::exactMousePosition;
}

const float Mouse::GetXPercentage()
{
	return InputDevice::mousePositionPercentage.x;
}

const float Mouse::GetYPercentage()
{
	return InputDevice::mousePositionPercentage.y;
}

const FPosition Mouse::GetPositionPercentage()
{
	return InputDevice::mousePositionPercentage;
}

const float Mouse::GetScroll()
{
	return InputDevice::scrollDirection;
}

