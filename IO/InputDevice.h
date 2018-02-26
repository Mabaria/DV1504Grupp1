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
	Q,
	E,
	Shift,
	Ctrl,
	Space,
	Tab,
	Backspace,
	One,
	Two,
	Three,
	Four,
};

enum Buttons
{
	Left,
	Right,
	ScrollPress,
};

struct Position
{
	int x, y;
};
struct FPosition
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
	//static bool buttonsActive[3];
	//static bool buttonsPressed[3];
	static char buttonsActive;
	static char buttonsPrevious;

	static Position mousePosition;
	static FPosition mousePositionPercentage;
	static Position exactMousePosition;
	static FPosition exactMousePositionPercentage;
	static float scrollDirection;

	static void SetMouseButton(const Buttons button, const bool is_pressed);
	static void SetMousePositionPercentage(const float x, const float y);
	static void SetMousePosition(const int x, const int y);
	static void SetExactMousePosition(const int x, const int y);
	static void SetExactMousePositionPercentage(const float x, const float y);

	static void SetMouseScroll(const float scroll);
private:

};
