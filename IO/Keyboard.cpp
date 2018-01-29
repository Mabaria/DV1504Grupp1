#include "Keyboard.h"

const bool Keyboard::IsKeyPressed(const Keys key)
{
	bool flag = false;

	if (InputDevice::keysPressed[key]
		&& !InputDevice::keysActive[key])
	{
		flag
			= InputDevice::keysActive[key]
			= InputDevice::keysPressed[key];
	}

	return flag;
}

const bool Keyboard::IsKeyDown(const Keys key)
{
	return InputDevice::keysPressed[key];
}