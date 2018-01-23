#include "stdafx.h"
#include "Test_Graphics.h"

void Test_Window()
{
	Window window(L"BIS", 1280, 720);

	window.Open();

	// To see that the value is stored correctly
	int width = window.GetClientSize().width;
	HWND hwnd = window.GetWindow();

	while (window.IsOpen())
	{
		window.Update();

		// Print size
		std::cout << window.GetClientSize().width << std::endl;
		std::cout << window.GetClientSize().height << std::endl;
		// The last 2 sizes will be null values since the window is destroyed
	}


	std::cout << width << std::endl;

	system("pause");

}