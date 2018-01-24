#include "Panel3D.h"

Panel3D::Panel3D(int width, int height, int top, int left, HWND handle)
	:Panel(width, height, top, left, handle), mDirect3D(handle, width, height)
{
	this->mPanelWindow = CreateWindowEx(
		0,
		L"PanelWindowClass", 
		(LPCTSTR) NULL, 
		WS_CHILD | WS_BORDER, 
		this->mLeft, 
		this->mTop, 
		this->mWidth, 
		this->mHeight, 
		handle, 
		0, 
		GetModuleHandle(0),
		0);
	
}

Panel3D::~Panel3D()
{
}

D3D11 & Panel3D::rGetDirect3D()
{
	return this->mDirect3D;
}

void Panel3D::Update()
{

}
