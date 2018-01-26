#include "Test_Panel.h"

void Test_Panel()
{
	Window window(L"Test_Window", 1280, 720);
	Panel3D panel(100, 100, 0, 0, window.GetWindow());	
}