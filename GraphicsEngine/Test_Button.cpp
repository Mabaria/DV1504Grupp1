#include "Test_Button.h"
#include "../UI/UIElements/Panel2D.h"
#include "../UI/UIElements/Panel3D.h"

void Test_Button()
{
	Window window(L"Button_Test", 1280, 720);
	Direct2D D2Test(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	Button button(&D2Test, "../../Models/testFile.bmp", 100, 100, 200, 200), 
		button2(&D2Test, "../../Models/pepehands.jpg", 205, 100, 305, 200);
	Mouse mouse;
	window.Open();
	
	while (window.IsOpen())
	{
		window.Update();
		
		if (Mouse::GetPositionPercentage().x <
			button.GetBoundingBoxPercentage().right &&
			mouse.GetPositionPercentage().x >
			button.GetBoundingBoxPercentage().left &&
			mouse.GetPositionPercentage().y <
			button.GetBoundingBoxPercentage().bottom &&
			mouse.GetPositionPercentage().y >
			button.GetBoundingBoxPercentage().top &&
			mouse.IsButtonDown(Buttons::Left))
		{
			button.SetButtonStatus(BUTTON_STATE::CLICKED);
			
		}
		else if (mouse.GetPositionPercentage().x <
			button.GetBoundingBoxPercentage().right &&
			mouse.GetPositionPercentage().x >
			button.GetBoundingBoxPercentage().left &&
			mouse.GetPositionPercentage().y <
			button.GetBoundingBoxPercentage().bottom &&
			Mouse::GetPositionPercentage().y >
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
		button.DrawButton();
		button2.DrawButton();
		D2Test.GetpRenderTarget()->EndDraw();	
	}
}
