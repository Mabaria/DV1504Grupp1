#include "Panel2D.h"
#include "../../IO/Mouse.h"

Panel2D::Panel2D(int width, int height, int top, int left, HWND handle, LPCTSTR title)
	:Panel(width, height, top, left, handle)
{
	this->mPanelWindow = CreateWindowEx(
		0,
		title,
		title,
		WS_CHILD | WS_BORDER,
		left,
		top,
		width,
		height,
		handle,
		0,
		GetModuleHandle(0),
		0);
	this->mDirect2D = new Direct2D(mPanelWindow, width, height);
	ShowWindow(this->mPanelWindow, SW_NORMAL);
}

Panel2D::~Panel2D()
{
	delete this->mDirect2D;
}

void Panel2D::AddButton(int width,
	int height,
	int top,
	int left,
	std::string imageFilePath,
	std::string buttonName)
{
	this->mButtonNames.push_back(buttonName);
	Button newButton(*this->mDirect2D,
		imageFilePath,
		left,
		top,
		left + width,
		top + height);
	this->mButtonVector.push_back(newButton);
}

void Panel2D::AddTextbox(int width, int height, int top, int left, LPCTSTR name)
{
	// Push textbox into list of UI elements.
}

void Panel2D::Update()
{
	for (std::vector<Button>::iterator it = this->mButtonVector.begin();
		it != this->mButtonVector.end();
		it++)
	{
		if (Mouse::IsButtonDown(Buttons::Left))
			int i = 0;

		if (Mouse::GetPositionPercentage().x <
			it->GetBoundingBoxPercentage().right &&
			Mouse::GetPositionPercentage().x >
			it->GetBoundingBoxPercentage().left &&
			Mouse::GetPositionPercentage().y <
			it->GetBoundingBoxPercentage().bottom &&
			Mouse::GetPositionPercentage().y >
			it->GetBoundingBoxPercentage().top)
		{
			if (Mouse::IsButtonDown(Buttons::Left))
			{
				it->SetButtonStatus(BUTTON_STATE::CLICKED);
				Mouse::GetPosition();
			}
			else
				it->SetButtonStatus(BUTTON_STATE::HOVER);
		}
		else
		{
			it->SetButtonStatus(BUTTON_STATE::IDLE);
		}

	}
}

void Panel2D::Draw()
{
	this->mDirect2D->GetpRenderTarget()->BeginDraw();
	this->mDirect2D->GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::HotPink));
	// Draw all the buttons in the panel
	for (std::vector<Button>::iterator it = this->mButtonVector.begin(); it != this->mButtonVector.end(); it++)
	{
		it->DrawButton();
	}
	this->mDirect2D->GetpRenderTarget()->EndDraw();
}
