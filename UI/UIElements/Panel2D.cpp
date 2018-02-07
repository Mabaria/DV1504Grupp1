#include "Panel2D.h"
#include "../../IO/Mouse.h"
#include <string>

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
	this->mButtonNames.push_back(buttonName); // Add to names list
	Button newButton(*this->mDirect2D,
		imageFilePath,
		left,
		top,
		left + width,
		top + height);
	this->mButtonVector.push_back(newButton); // Add button
}

void Panel2D::AddTextbox(int width, int height, int top, int left, LPCTSTR name)
{
	// Push textbox into list of UI elements.
}

Button * Panel2D::GetButtonByName(std::string name)
{
	Button *to_return = nullptr; // Default return in nullptr
	unsigned int count = 0;
	std::vector<std::string>::iterator it;

	for (it = this->mButtonNames.begin();
		it != this->mButtonNames.end();
		++it, count++)
	{
		if (name.compare(*it) == 0) // Button with correct name found
		{
			to_return = &this->mButtonVector[count]; // Return pointer to button
			it = this->mButtonNames.end() - 1; // Set iterator to end
			// -1 because incrementation is performed after this
			// incrementing on .end() is a baaad idea
		}
	}
	return to_return;
}

Button * Panel2D::GetButtonByIndex(unsigned int index)
{
	Button *to_return = nullptr;
	if (index <= this->mButtonVector.size()) // Bounds check
	{
		to_return = &this->mButtonVector[index];
	}
	return to_return;
}

void Panel2D::Update()
{
	this->mUpdateButtons();
}

void Panel2D::Draw()
{
	this->mDirect2D->GetpRenderTarget()->BeginDraw();
	this->mDirect2D->GetpRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	// Draw all the buttons in the panel
	for (std::vector<Button>::iterator it = this->mButtonVector.begin(); it != this->mButtonVector.end(); it++)
	{
		it->DrawButton();
	}
	this->mDirect2D->GetpRenderTarget()->EndDraw();
}

void Panel2D::mUpdateButtons()
{
	if (this->mIsMouseInsidePanel()) /* Check if mouse is inside panel,
									 if not there is no chance of any buttons
									 being pressed. */
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
				it->GetBoundingBoxPercentage().top) /* Classic bounds check */
			{
				if (Mouse::IsButtonDown(Buttons::Left))
				{
					it->SetButtonStatus(BUTTON_STATE::CLICKED);
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
	else // Mouse outside panel, make sure buttons are idle
	{
		for (std::vector<Button>::iterator it = this->mButtonVector.begin();
			it != this->mButtonVector.end();
			it++)
		{
			it->SetButtonStatus(BUTTON_STATE::IDLE);
		}
	}
}

bool Panel2D::mIsMouseInsidePanel()
{
	if (Mouse::IsButtonDown(Buttons::Left))
		int i = 0;
	RECT window_rect;
	GetWindowRect(this->mPanelWindow, &window_rect);
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	return PtInRect(&window_rect, mouse_pos);

}
