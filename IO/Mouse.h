#pragma once

#include "InputDevice.h"

class Mouse
{
public:
	static const bool IsButtonPressed(const Buttons button);
	static const bool IsButtonDown(const Buttons button);
	
	static const float GetMouseX();
	static const float GetMouseY();
	static const Position GetMousePosition();
	
	static const float GetMouseXPercentage();
	static const float GetMouseYPercentage();
	static const Position GetMousePositionPercentage();
	

private:

};