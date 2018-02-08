#pragma once

#include <Windows.h>
#include "Panel.h"
#include "Button.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include <vector>

class TextBox
{
public:
	TextBox(
	Direct2D *D2D1Panel,
		int left,
		int top,
		int right, 
		int bottom) {};
	void SetText(std::string text) {};
	void DrawTextBox() {};
private:
};


class Panel2D : public Panel
{
public:
	Panel2D(
		int width, 
		int height, 
		int top, 
		int left, 
		HWND handle, 
		LPCTSTR title);
	~Panel2D();

	void AddButton(
		int width,
		int height,
		int top,
		int left,
		std::string imageFilePath,
		std::string buttonName);

	void AddTextbox(
		int width, 
		int height, 
		int top, 
		int left, 
		std::string text,
		std::string name);

	Button* GetButtonByName(std::string name);
	Button* GetButtonByIndex(unsigned int index);
	
	TextBox* GetTextBoxByName(std::string name);
	TextBox* GetTextBoxByIndex(unsigned int index);

	void Update();
	void Draw();

private:
	Direct2D *mDirect2D;
	bool mIsMouseInsidePanel();
	
	void mUpdateButtons();
	std::vector<Button> mButtonVector;
	std::vector<std::string> mButtonNames;

	void mUpdateTextBoxes();
	std::vector<TextBox> mTextBoxVector;
	std::vector<std::string> mTextBoxNames;
};
