#include "Test_TextBox.h"

void Test_Text()
{
	Window window(L"Text_Test", 1280, 720);
	Direct2D D2Test(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height);
	Button butt(&D2Test,"",0,0,0,0);
	TextBox testBox(&D2Test, 0, 0, 100, 100);
	TextBox testBox2(&D2Test, 0, 0, 100, 100);
	testBox.SetText("");
	testBox.SetText("Fuck you robin");
	window.Open();
	std::string testText = "";
	while (window.IsOpen())
	{
		testText += "h";
		testBox.SetText(testText);
		testBox.SetTextBoxSize((int)testBox.GetTextBoxSize().left + 1,
			(int)testBox.GetTextBoxSize().top + 1,
			(int)testBox.GetTextBoxSize().right + 2,
			(int)testBox.GetTextBoxSize().bottom + 2);
		window.Update();
		D2Test.GetpRenderTarget()->BeginDraw();
		D2Test.GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
		testBox.DrawTextBox();
		testBox2.DrawTextBox();
		D2Test.GetpRenderTarget()->EndDraw();
	}

}
