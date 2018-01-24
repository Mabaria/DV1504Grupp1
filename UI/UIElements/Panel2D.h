#pragma once

#include <Windows.h>
#include "Panel.h"

class Panel2D : public Panel
{
public:
	Panel2D(int width, int height, int top, int left, HWND handle);
	~Panel2D();

	void AddButton(int width, int height, int top, int left, LPCTSTR name, LPCTSTR text);
	void AddTextbox();


private:
	// List of UI elements.
};
