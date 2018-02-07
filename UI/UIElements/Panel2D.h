#pragma once

#include <Windows.h>
#include "Panel.h"
#include "Button.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include <vector>

class Panel2D : public Panel
{
public:
	Panel2D(int width, int height, int top, int left, HWND handle, LPCTSTR title);
	~Panel2D();

	void AddButton(int width,
		int height,
		int top,
		int left,
		std::string imageFilePath,
		std::string buttonName);
	void AddTextbox(int width, int height, int top, int left, LPCTSTR name);
	Button* GetButtonByName(std::string name);
	Button* GetButtonByIndex(unsigned int index);
	void Update();
	void Draw();

private:
	Direct2D *mDirect2D;
	void mUpdateButtons();
	HWND mPanelWindow;
	bool mIsMouseInsidePanel();
	std::vector<Button> mButtonVector;
	std::vector<std::string> mButtonNames;
};
