#include "Panel2D.h"

Panel2D::Panel2D(int width, int height, int top, int left, HWND handle, LPCTSTR title)
	:Panel(width, height, top, left, handle, title)
{
	// Setup list of UI elements.
}

Panel2D::~Panel2D()
{
	// Kill list of UI elements.
}

void Panel2D::AddButton(int width, int height, int top, int left, LPCTSTR name, LPCTSTR text)
{
	// Push button into list of UI elements.
}

void Panel2D::AddTextbox(int width, int height, int top, int left, LPCTSTR name)
{
	// Push textbox into list of UI elements.
}

void Panel2D::Update()
{
}
