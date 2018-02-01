#include "Test_Button.h"

void Test_Button()
{
	Window window(L"Button_Test", 1280, 720);
	Direct2D D2Test;
	Button button, button2;
	Mouse mouse;
	D2Test.CreateRenderTarget(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	button.CreateButton(D2Test, "../../Models/testFile.bmp", 100, 100, 200, 200);
	button2.CreateButton(D2Test, "../../Models/pepehands.jp", 400, 100, 600, 300);
	window.Open();
	
	
	while (window.IsOpen())
	{
		window.Update();
		
		if (mouse.GetMousePositionPercentage().x <
			button.GetBoundingBoxPercentage().right &&
			mouse.GetMousePositionPercentage().x >
			button.GetBoundingBoxPercentage().left &&
			mouse.GetMousePositionPercentage().y <
			button.GetBoundingBoxPercentage().bottom &&
			mouse.GetMousePositionPercentage().y >
			button.GetBoundingBoxPercentage().top &&
			mouse.IsButtonDown(Buttons::Left))
		{
			button.SetButtonStatus(BUTTON_STATE::CLICKED);
		}
		else if (mouse.GetMousePositionPercentage().x <
			button.GetBoundingBoxPercentage().right &&
			mouse.GetMousePositionPercentage().x >
			button.GetBoundingBoxPercentage().left &&
			mouse.GetMousePositionPercentage().y <
			button.GetBoundingBoxPercentage().bottom &&
			mouse.GetMousePositionPercentage().y >
			button.GetBoundingBoxPercentage().top)
		{
			button.SetButtonStatus(BUTTON_STATE::HOVER);
		}
		else
		{
			button.SetButtonStatus(BUTTON_STATE::IDLE);
		}
		D2Test.GetpRenderTarget()->BeginDraw();
		D2Test.GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
		button.DrawButton(D2Test);
		button2.DrawButton(D2Test);
		D2Test.GetpRenderTarget()->EndDraw();	
	}
}
