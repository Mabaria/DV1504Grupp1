#pragma once

#include "InputDevice.h"

class Mouse
{
public:
	static const bool IsButtonPressed(const Buttons button);
	static const bool IsButtonDown(const Buttons button);
	
	static const int GetX();
	static const int GetY();
	static const int GetExactX();
	static const int GetExactY();
	static const Position GetPosition();
	static const Position GetExactPosition();
	
	static const float GetXPercentage();
	static const float GetYPercentage();
	static const FPosition GetPositionPercentage();
	
	static const float GetScroll();

private:

};