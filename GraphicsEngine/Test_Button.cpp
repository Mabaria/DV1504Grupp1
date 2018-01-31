#include "Test_Button.h"

void Test_Button()
{
	Window window(L"Button_Test", 1280, 720);
	Direct2D D2Test;
	Button button;
	D2Test.CreateRenderTarget(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	button.CreateButton(D2Test, "pepehands.jpg", 0, 0, 500, 500);
	button.DrawButton(D2Test);
}
