#include "stdafx.h"
#include "Test_Graphics.h"

void Test_Window()
{
	Window window(L"BIS", 1280, 720);

	window.Open();

	// To see that the value is stored correctly
	int width = window.GetClientSize().width;

	while (window.IsOpen())
	{
		window.Update();

		// Print size
		std::cout << window.GetClientSize().width << std::endl;
		std::cout << window.GetClientSize().height << std::endl;
	}

	std::cout << width << std::endl;

	system("pause");

}