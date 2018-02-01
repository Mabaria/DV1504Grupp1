#pragma once

#include "InputDevice.h"

class Mouse
{
public:
	static const bool IsButtonPressed(const Buttons button);
	static const bool IsButtonDown(const Buttons button);
	
	static const float GetX();
	static const float GetY();
	static const Position GetPosition();
	
	static const float GetXPercentage();
	static const float GetYPercentage();
	static const Position GetPositionPercentage();
	
	static const float GetScroll();

private:

};