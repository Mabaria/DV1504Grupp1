#include "Test_Input.h"

#include <string>
#include <iostream>

void Test_Input()
{
	Window window(L"Test_Input", 1280, 720);


	int nrOfKeys = 12;
	Keys keys[] = {
		Keys::Enter,
		Keys::W,
		Keys::A,
		Keys::S,
		Keys::D,
		Keys::Ctrl,
		Keys::Shift,
		Keys::Space,
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
		"Ctrl",
		"Shift",
		"Space",
		"1",
		"2",
		"3",
		"4",
	};
	

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
			std::cout << Mouse::GetMouseXPercentage() << std::endl;
			std::cout << Mouse::GetMouseYPercentage() << std::endl << std::endl;
		}
	}
}