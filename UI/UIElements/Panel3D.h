#pragma once

#include <Windows.h>
#include "Panel.h"
#include "../../BIS/BIS/Direct3D.h"
#include "MeshObject.h"

class Panel3D : public Panel
{
public:
	Panel3D(int width, int height, int top, int left, HWND handle);
	~Panel3D();
	
	D3D11& rGetDirect3D();


	void Update();

private:
	D3D11 mDirect3D;
	// Mesh object / drawable
	HWND mPanelWindow;
};