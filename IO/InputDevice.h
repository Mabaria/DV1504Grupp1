#pragma once

#define NUMBER_OF_KEYS 256

enum Keys 
{
	Esc,
	Enter,
	W,
	A,
	S,
	D,
	Shift,
	Ctrl,
	Space,
	One,
	Two,
	Three,
	Four,
};

enum Buttons
{
	Left,
	Right,
};

struct Position
{
	float x, y;
};

class InputDevice
{
public:
	// Keyboard input
	static bool keysPressed[NUMBER_OF_KEYS];
	static bool keysActive[NUMBER_OF_KEYS];
	static void SetKey(const Keys key, const bool is_pressed);

	// Mouse input
	static Position mousePosition;
	static Position mousePositionPercentage;
	static bool buttonsPressed[2];
	static bool buttonsActive[2];

	static void SetMouseButton(const Buttons button, const bool is_pressed);
	static void SetMousePositionPercentage(const float x, const float y);
	static void SetMousePosition(const float x, const float y);

private:

};
