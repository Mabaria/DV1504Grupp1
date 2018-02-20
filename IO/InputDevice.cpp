#include "InputDevice.h"

bool InputDevice::keysPressed[]					= { 0 };
bool InputDevice::keysActive[]					= { 0 };
//bool InputDevice::buttonsPressed[]				= { 0 };
//bool InputDevice::buttonsActive[]				= { 0 };
char InputDevice::buttonsActive = 0;
char InputDevice::buttonsPrevious = 0;
Position InputDevice::mousePosition				= { 0 };
Position InputDevice::mousePositionPercentage	= { 0 };
Position InputDevice::exactMousePosition = { 0 };
Position InputDevice::exactMousePositionPercentage = { 0 };
float InputDevice::scrollDirection				= 0.0f;

void InputDevice::SetKey(const Keys key, const bool is_pressed)
{
	if (!is_pressed)
	{
		InputDevice::keysActive[key] = false;
	}

	InputDevice::keysPressed[key] = is_pressed;
}

void InputDevice::SetMouseButton(const Buttons button, const bool is_pressed)
{
	/*if (!is_pressed)
	{
		InputDevice::buttonsActive[button] = false;
	}

	InputDevice::buttonsPressed[button] = is_pressed;*/
	if(is_pressed)
		InputDevice::buttonsActive |= 1 << button;
	else
		InputDevice::buttonsActive &= ~(1 << button);
}

void InputDevice::SetMousePosition(const float x, const float y)
{
	InputDevice::mousePosition.x = x;
	InputDevice::mousePosition.y = y;
}

void InputDevice::SetExactMousePosition(const float x, const float y)
{
	InputDevice::exactMousePosition.x = x;
	InputDevice::exactMousePosition.y = y;
}

void InputDevice::SetExactMousePositionPercentage(const float x, const float y)
{
	InputDevice::exactMousePositionPercentage.x = x;
	InputDevice::exactMousePositionPercentage.y = y;
}

void InputDevice::SetMousePositionPercentage(const float x, const float y)
{
	InputDevice::mousePositionPercentage.x = x;
	InputDevice::mousePositionPercentage.y = y;
}

void InputDevice::SetMouseScroll(const float scroll)
{
	InputDevice::scrollDirection = scroll;
}