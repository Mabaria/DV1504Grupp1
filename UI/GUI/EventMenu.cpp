#include "EventMenu.h"
#include <iostream>

EventMenu::EventMenu()
{
	this->mMenuWidth = 400;
	this->mMenuHeight = 300;
	this->mParentPanelWidth = 0;
	this->mParentPanelHeight = 0;

	this->mVisible = false;
	this->mButtonFocus = false;
	this->mDraggingWindow = false;
	this->mpEventLog = nullptr;
	this->mpPanel = nullptr;
	this->mpActiveRoom = nullptr;
}

EventMenu::~EventMenu()
{
	if (this->mpPanel != nullptr)
		delete this->mpPanel;
}

bool EventMenu::Init(int parentWidth,
	int parentHeight,
	EventLog *pEventLog,
	LPCTSTR windowName,
	HWND *pWindow)
{
	this->mParentPanelWidth = parentWidth;
	this->mParentPanelHeight = parentHeight;

	this->mMenuHeight = this->mParentPanelHeight;

	this->mpEventLog = pEventLog;
	this->mpPanel = new Panel2D(
		this->mMenuWidth,										// Width
		this->mMenuHeight,										// Height
		this->mParentPanelHeight / 2 - this->mMenuHeight / 2,	// Top
		this->mParentPanelWidth / 2 - this->mMenuWidth / 2,		// Left
		*pWindow,
		windowName);

	this->mpPanel->LoadImageToBitmap("../../Models/Button01.png", "InjuryOff");
	this->mpPanel->LoadImageToBitmap("../../Models/Button02.png", "GasOff");
	this->mpPanel->LoadImageToBitmap("../../Models/Button03.png", "WaterOff");
	this->mpPanel->LoadImageToBitmap("../../Models/Button04.png", "FireOff");
	this->mpPanel->LoadImageToBitmap("../../Models/Button06.png", "InjuryOn");
	this->mpPanel->LoadImageToBitmap("../../Models/Button07.png", "GasOn");
	this->mpPanel->LoadImageToBitmap("../../Models/Button08.png", "WaterOn");
	this->mpPanel->LoadImageToBitmap("../../Models/Button09.png", "FireOn");
	this->mpPanel->LoadImageToBitmap("../../Models/Exit.png", "Exit");

	this->mpPanel->LoadImageToBitmap("../../Models/Symbols.png", "Symbols");
	
	this->InitButtons();

	//! DET GÅR INTE ATT GÖMMA FÖNSTRET I BÖRJAN.
	this->mpPanel->Hide();

	return true;
}

bool EventMenu::OpenAt(Room *pRoom)
{
	int margin = this->mParentPanelWidth / 24;
	int posX = this->mParentPanelWidth - this->mMenuWidth;
	int posY = 0;
	//FPosition mousePos = Mouse::GetPositionPercentage();
	//mousePos.x = mousePos.x * this->mParentPanelWidth;
	//mousePos.y = mousePos.y * this->mParentPanelHeight;
	/*Position mousePos = Mouse::GetPosition();

	if (mousePos.x + margin + this->mMenuWidth < this->mParentPanelWidth)
		posX = mousePos.x + margin;
	else
		posX = mousePos.x - margin - this->mMenuWidth;

	if (mousePos.y + margin + this->mMenuHeight < this->mParentPanelHeight)
		posY = mousePos.y + margin;
	else
		posY = mousePos.y - margin - this->mMenuHeight;*/

	this->mpPanel->SetLeft(posX);
	this->mpPanel->SetTop(posY);
	this->mpPanel->UpdateWindowPos();
	this->mpPanel->Show();

	this->mVisible = true;

	this->mpActiveRoom = pRoom;

	// TODO: Following functionallity does not work because GetActiveEvents
	// does not work as intended. Not too important feature anyway
	//std::vector<LogEvent*> test = pRoom->GetActiveEvents();
	//for (int i = 0; i < pRoom->GetActiveEvents().size(); i++)
	//{
	//	switch (pRoom->GetActiveEvents()[i]->GetType())
	//	{
	//	case Event::Type::Fire:
	//		this->mpPanel->GetButtonByName("Fire")->SetBitmap(
	//			this->mpPanel->GetBitmapByName("FireOn"));
	//		break;
	//	case Event::Type::Water:
	//		this->mpPanel->GetButtonByName("Water")->SetBitmap(
	//			this->mpPanel->GetBitmapByName("WaterOn"));
	//		break;
	//	case Event::Type::Gas:
	//		this->mpPanel->GetButtonByName("Gas")->SetBitmap(
	//			this->mpPanel->GetBitmapByName("GasOn"));
	//		break;
	//	case Event::Type::Injury:
	//		this->mpPanel->GetButtonByName("Injury")->SetBitmap(
	//			this->mpPanel->GetBitmapByName("InjuryOn"));
	//		break;
	//	}
	//}

	return true;
}

bool EventMenu::Update()
{
	this->mpPanel->Update();
	if (Mouse::IsButtonPressed(Buttons::Left)
		&& !this->mDraggingWindow
		&& !this->mButtonFocus
		&& this->mpPanel->IsMouseInsidePanel()
		&& !this->mpPanel->GetButtonOcclude())
	{
		this->mDraggingWindow = true;
		this->mDragX = Mouse::GetExactX() - this->mpPanel->GetLeft();
		this->mDragY = Mouse::GetExactY() - this->mpPanel->GetTop();
	}
	else if (!Mouse::IsButtonDown(Buttons::Left))
	{
		this->mButtonFocus = false;
		this->mDraggingWindow = false;
	}

	//if (this->mDraggingWindow)
	//{
	//	int left = Mouse::GetExactX() - this->mDragX;
	//	if (left + this->mMenuWidth > this->mParentPanelWidth)
	//		left = this->mParentPanelWidth - this->mMenuWidth;
	//	else if (left < 0)
	//		left = 0;

	//	int top = Mouse::GetExactY() - this->mDragY;
	//	if (top + this->mMenuHeight > this->mParentPanelHeight)
	//		top = this->mParentPanelHeight - this->mMenuHeight;
	//	else if (top < 0)
	//		top = 0;

	//	this->mpPanel->SetLeft(left);
	//	this->mpPanel->SetTop(top);
	//	this->mpPanel->UpdateWindowPos();
	//}
	return true;
}

bool EventMenu::Draw() const
{
	if (this->mVisible)
		this->mpPanel->Draw();
	else
		this->mpPanel->Hide();
	return true;
}

bool EventMenu::IsMouseInsidePanel()
{
	return this->mpPanel->IsMouseInsidePanel();
}

bool EventMenu::IsVisible()
{
	return this->mpPanel->IsVisible();
}

void EventMenu::Update( Button *attribute)
{
	if (this->mpPanel->IsVisible())
	{
		std::cout << "Clicked on ";
		std::string button_name = attribute->GetName();
		if (button_name.compare("Exit") == 0)
		{
			this->mpPanel->Hide();
			this->mVisible = false;
		}
		else
		{
			if (button_name.compare("Fire") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Fire);
				this->mLastClicked = Event::Fire;
			}
			else if (button_name.compare("Gas") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Gas);
				this->mLastClicked = Event::Gas;
			}
			if (button_name.compare("Water") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Water);
				this->mLastClicked = Event::Water;
			}
			else if (button_name.compare("Injury") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Injury);
				this->mLastClicked = Event::Injury;
			}
			this->NotifyObservers(this->mpActiveRoom);
		}
	}
}

Event::Type EventMenu::GetLastClicked()
{
	return this->mLastClicked;
}

void EventMenu::InitButtons()
{
	int margin = this->mMenuWidth / 6;
	int padding = this->mMenuWidth / 6;
	int actionMargin = this->mMenuWidth / 15;
	int buttonSize = this->mMenuWidth / 4;

	// Event buttons

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin,
		margin,
		this->mpPanel->GetBitmapByName("FireOff"),
		"Fire");
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + buttonSize + padding,
		margin,
		this->mpPanel->GetBitmapByName("WaterOff"),
		"Water"); 
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin,
		margin + buttonSize + padding,
		this->mpPanel->GetBitmapByName("GasOff"),
		"Gas");
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + buttonSize + padding,
		margin + buttonSize + padding,
		this->mpPanel->GetBitmapByName("InjuryOff"),
		"Injury");

	// Action buttons
	// Ordered:
	// 1 2 3
	// 4 5 6
	// 7 8 9

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2,
		actionMargin,
		this->mpPanel->GetBitmapByName("InjuryOn"),
		"Injured_Moved");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("InjuryOn"),
		"Injured_Treated");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("InjuryOn"),
		"Injured_Reported");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + actionMargin + buttonSize,
		actionMargin,
		this->mpPanel->GetBitmapByName("GasOn"),
		"Hole_In_Bulk");


	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + actionMargin + buttonSize,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("GasOn"),
		"Ventilation_In");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + actionMargin + buttonSize,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("GasOn"),
		"Ventilation_Out");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		actionMargin,
		this->mpPanel->GetBitmapByName("FireOn"),
		"Cooling_Wall");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("FireOn"),
		"Supporting_Wall");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (buttonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("FireOn"),
		"Damaged_Bulk");

	this->mpPanel->AddButton(
		this->mMenuWidth / 9,
		this->mMenuWidth / 9,
		0,
		this->mMenuWidth - this->mMenuWidth / 9,
		this->mpPanel->GetBitmapByName("Exit"),
		"Exit");
	this->mpPanel->GetButtonByName("Fire")->AddObserver(this);
	this->mpPanel->GetButtonByName("Gas")->AddObserver(this);
	this->mpPanel->GetButtonByName("Water")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injury")->AddObserver(this);
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);

	//! ADD ACTION BUTTON OBSERVERS HERE
}
