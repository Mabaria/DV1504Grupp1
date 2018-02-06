#include "Test_Button.h"
#include "../UI/UIElements/Panel2D.h"

void Test_Button()
{
	Window window(L"Button_Test", 1280, 720);
	Panel2D testPanel(500, 500, 0, 0, window.GetWindow(), L"Button_Test");
	testPanel.AddButton(100, 100, 0, 0, "../../Models/FireButton.png", "FireButton");
	testPanel.AddButton(100, 100, 100, 0, "../../Models/FireButton.png", "FireButton2");
	testPanel.AddButton(100, 100, 200, 0, "../../Models/FireButton.png", "FireButton3");
	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		testPanel.Draw();

	}
	//Direct2D D2Test(
	//	window.GetWindow(),
	//	window.GetClientSize().width,
	//	window.GetClientSize().height);
	//Button button(D2Test, "../../Models/FireButton.png", 100, 100, 200, 200),
	//	button2(
	//		D2Test, 
	//		"../../Models/testFile.bmp", 
	//		button.GetButtonsize().left / 1, 
	//		button.GetButtonsize().top / 1,
	//		button.GetButtonsize().right/1.33, 
	//		button.GetButtonsize().bottom/1.33);
	//Mouse mouse;
	//float speed = 1.0f;
	//window.Open();
	//while (window.IsOpen())
	//{
	//	window.Update();
	//	button.SetButtonsize(
	//		button.GetButtonsize().left + speed, 
	//		button.GetButtonsize().top + speed, 
	//		button.GetButtonsize().right + speed, 
	//		button.GetButtonsize().bottom + speed);

	//	button2.SetButtonsize(
	//		button2.GetButtonsize().left + speed, 
	//		button2.GetButtonsize().top + speed, 
	//		button2.GetButtonsize().right + speed, 
	//		button2.GetButtonsize().bottom + speed);

	//	if (Mouse::GetPositionPercentage().x <
	//		button.GetBoundingBoxPercentage().right &&
	//		mouse.GetPositionPercentage().x >
	//		button.GetBoundingBoxPercentage().left &&
	//		mouse.GetPositionPercentage().y <
	//		button.GetBoundingBoxPercentage().bottom &&
	//		mouse.GetPositionPercentage().y >
	//		button.GetBoundingBoxPercentage().top &&
	//		mouse.IsButtonDown(Buttons::Left))
	//	{
	//		button.SetButtonStatus(BUTTON_STATE::CLICKED);
	//		//button.getBitmapPointer()->Release();
	//		//button.LoadImageToBitmap("../../Models/pepehands.jpg");
	//		//window.Close();
	//	}
	//	else if (mouse.GetPositionPercentage().x <
	//		button.GetBoundingBoxPercentage().right &&
	//		mouse.GetPositionPercentage().x >
	//		button.GetBoundingBoxPercentage().left &&
	//		mouse.GetPositionPercentage().y <
	//		button.GetBoundingBoxPercentage().bottom &&
	//		Mouse::GetPositionPercentage().y >
	//		button.GetBoundingBoxPercentage().top)
	//	{
	//		button.SetButtonStatus(BUTTON_STATE::HOVER);
	//	}
	//	else
	//	{
	//		button.SetButtonStatus(BUTTON_STATE::IDLE);
	//	}
	//	D2Test.GetpRenderTarget()->BeginDraw();
	//	D2Test.GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	//	button.DrawButton();
	//	button2.DrawButton();
	//	D2Test.GetpRenderTarget()->EndDraw();	
	//}
}
