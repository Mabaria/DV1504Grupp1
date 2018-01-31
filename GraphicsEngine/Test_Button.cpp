#include "Test_Button.h"

void Test_Button()
{
	Window window(L"Button_Test", 1280, 720);
	Direct2D D2Test;
	Button button;
	Mouse mouse;
	D2Test.CreateRenderTarget(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	window.Open();
	
	while (window.IsOpen())
	{
		window.Update();
		button.CreateButton(D2Test, "pepehands.jpg", 0, 0, 500, 500);

		D2Test.GetpRenderTarget()->BeginDraw();
		if (mouse.GetMousePosition().x < 500 && mouse.GetMousePosition().y < 500)
		{
			button.LoadImageToBitmap(D2Test, "../../Models/Fern.jpg");
		}
		else
			button.LoadImageToBitmap(D2Test, "../../Models/pepehands.jpg");
		button.DrawButton(D2Test);
		D2Test.GetpRenderTarget()->EndDraw();	
	}
}
