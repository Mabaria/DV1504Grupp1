#include "Test_Input.h"

#include <string>
#include <iostream>

void Test_Input()
{
	Window window(L"Test_Input", 1280, 720);


	int nrOfKeys = 16;
	Keys keys[] = {
		Keys::Enter,
		Keys::W,
		Keys::A,
		Keys::S,
		Keys::D,
		Keys::Q,
		Keys::E,
		Keys::Ctrl,
		Keys::Shift,
		Keys::Space,
		Keys::Tab,
		Keys::Backspace,
		Keys::One,
		Keys::Two,
		Keys::Three,
		Keys::Four,
	};

	std::string data[] = {
		"Enter",
		"W",
		"A",
		"S",
		"D",
		"Q",
		"E",
		"Ctrl",
		"Shift",
		"Space",
		"Tab",
		"Backspace",
		"1",
		"2",
		"3",
		"4",
	};

	int count = 0;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();

		if (Keyboard::IsKeyPressed(Keys::Esc))
		{
			window.Close();
		}

		for (int i = 0; i < nrOfKeys; i++)
		{
			if (Keyboard::IsKeyPressed(keys[i]))
			{
				std::cout << "Key: " << data[i] << " was pressed!" << std::endl;
			}
		}

		if (Mouse::IsButtonPressed(Buttons::Right) 
			|| Mouse::IsButtonDown(Buttons::Left))
		{
			std::cout << Mouse::GetXPercentage() << std::endl;
			std::cout << Mouse::GetYPercentage() << std::endl << std::endl;
		}

		if (Mouse::GetScroll() > 0)
		{
			count++;
			std::cout << count << std::endl;
		}

		if (Mouse::GetScroll() < 0)
		{
			count--;
			std::cout << count << std::endl;
		}
		
	}
}