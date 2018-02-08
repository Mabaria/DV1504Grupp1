#include "Test_TextBox.h"

void Test_Text()
{
	Window window(L"Text_Test", 1280, 720);
	Direct2D D2Test(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	Button butt(&D2Test,"",0,0,0,0);
	TextBox testBox(&D2Test, 0, 0, 1000, 1000);
	testBox.SetText("hejhejhejhejhejhejhejhejhejhejhejj lol");
	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		D2Test.GetpRenderTarget()->BeginDraw();
		D2Test.GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
		testBox.DrawTextBox();
		D2Test.GetpRenderTarget()->EndDraw();
	}

}
