#pragma once

#include "Panel.h"
#include <Windows.h>

class Panel3D : public Panel
{
public:
	Panel3D(int width, int height, int top, int left, HWND handle);
	~Panel3D();

private: 
	// Instance of D3D11
	// Mesh object / drawable
	HWND mPanelWindow;
};
