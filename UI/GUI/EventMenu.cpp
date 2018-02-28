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

	this->mActionMode = STANDARD;
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

	this->mpPanel->LoadImageToBitmap("../../Models/Action1.png", "Action1");
	this->mpPanel->LoadImageToBitmap("../../Models/Action2.png", "Action2");
	this->mpPanel->LoadImageToBitmap("../../Models/Action3.png", "Action3");
	this->mpPanel->LoadImageToBitmap("../../Models/Action4.png", "Action4");
	this->mpPanel->LoadImageToBitmap("../../Models/Action5.png", "Action5");
	this->mpPanel->LoadImageToBitmap("../../Models/Action6.png", "Action6");
	this->mpPanel->LoadImageToBitmap("../../Models/Action7.png", "Action7");
	this->mpPanel->LoadImageToBitmap("../../Models/Action8.png", "Action8");
	this->mpPanel->LoadImageToBitmap("../../Models/Action9.png", "Action9");
	
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
	//! Dragging removed due to new menu format
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
			if (this->mActionMode == NUMBERS) {
				this->mSwapActionMode(); // Menu closing, reset buttons
			}
			this->mVisible = false;
		}
		else
		{

			if (button_name.compare("Fire") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Fire);
				this->mLastClicked = Event::Fire;
				this->NotifyObservers(this->mpActiveRoom);
			}
			else if (button_name.compare("Gas") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Gas);
				this->mLastClicked = Event::Gas;
				this->NotifyObservers(this->mpActiveRoom);
			}
			else if (button_name.compare("Water") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Water);
				this->mLastClicked = Event::Water;
				this->NotifyObservers(this->mpActiveRoom);
			}
			else if (button_name.compare("Injury") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Injury);
				this->mLastClicked = Event::Injury;
				this->NotifyObservers(this->mpActiveRoom);
			}
			else if (this->mActionMode == STANDARD)
			{
				if (button_name.compare("Injured_Moved") == 0)
				{
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Treated") == 0)
				{
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Reported") == 0)
				{
					this->mSwapActionMode();
				}
				else if (button_name.compare("Hole_In_Bulk") == 0)
				{
					//TODO Add new action object
				}
				else if (button_name.compare("Ventilation_In") == 0)
				{
					//TODO Add new action object
				}
				else if (button_name.compare("Ventilation_Out") == 0)
				{
					//TODO Add new action object
				}
				else if (button_name.compare("Cooling_Wall") == 0)
				{
					//TODO Add new action object
				}
				else if (button_name.compare("Supporting_Wall") == 0)
				{
					//TODO Add new action object
				}
				else if (button_name.compare("Damaged_Bulk") == 0)
				{
					//TODO Add new action object
				}
			}
			else { //! ActionMode == NUMBERS
				if (button_name.compare("Injured_Moved") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Treated") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Reported") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Hole_In_Bulk") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Ventilation_In") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Ventilation_Out") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Cooling_Wall") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Supporting_Wall") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
				else if (button_name.compare("Damaged_Bulk") == 0)
				{
					//TODO Add new action object
					this->mSwapActionMode();
				}
			}
		}
	}
}

Event::Type EventMenu::GetLastClicked()
{
	return this->mLastClicked;
}

void EventMenu::mSwapActionMode()
{
	if (this->mActionMode == STANDARD)
	{
		this->mActionMode = NUMBERS;
		this->mpPanel->GetButtonByName("Injured_Moved")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Injured_Treated")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Injured_Reported")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Hole_In_Bulk")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Ventilation_In")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Ventilation_Out")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Cooling_Wall")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Supporting_Wall")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));

		this->mpPanel->GetButtonByName("Damaged_Bulk")->SetBitmap(
			this->mpPanel->GetBitmapByName("FireOn"));
	}
	else {
		this->mActionMode = STANDARD;

		// Reset all button bitmaps
		this->mpPanel->GetButtonByName("Injured_Moved")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action1"));

		this->mpPanel->GetButtonByName("Injured_Treated")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action2"));

		this->mpPanel->GetButtonByName("Injured_Reported")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action3"));

		this->mpPanel->GetButtonByName("Hole_In_Bulk")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action4"));

		this->mpPanel->GetButtonByName("Ventilation_In")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action5"));

		this->mpPanel->GetButtonByName("Ventilation_Out")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action6"));

		this->mpPanel->GetButtonByName("Cooling_Wall")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action7"));

		this->mpPanel->GetButtonByName("Supporting_Wall")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action8"));

		this->mpPanel->GetButtonByName("Damaged_Bulk")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action9"));
	}
}

void EventMenu::InitButtons()
{
	int margin = this->mMenuWidth / 10;
	int padding = this->mMenuWidth / 9;
	int actionMargin = this->mMenuWidth / 15;
	int buttonSize = this->mMenuWidth / 4;
	int eventButtonSize = this->mMenuWidth / 3;

	// Event buttons

	this->mpPanel->AddButton(
		eventButtonSize,
		eventButtonSize,
		margin,
		margin,
		this->mpPanel->GetBitmapByName("FireOff"),
		"Fire");
	this->mpPanel->AddButton(
		eventButtonSize,
		eventButtonSize,
		margin + eventButtonSize + padding,
		margin,
		this->mpPanel->GetBitmapByName("WaterOff"),
		"Water"); 
	this->mpPanel->AddButton(
		eventButtonSize,
		eventButtonSize,
		margin,
		this->mMenuWidth - margin - eventButtonSize,
		this->mpPanel->GetBitmapByName("GasOff"),
		"Gas");
	this->mpPanel->AddButton(
		eventButtonSize,
		eventButtonSize,
		margin + eventButtonSize + padding,
		this->mMenuWidth - margin - eventButtonSize,
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
		margin + (eventButtonSize + margin) * 2,
		actionMargin,
		this->mpPanel->GetBitmapByName("Action1"),
		"Injured_Moved");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("Action2"),
		"Injured_Treated");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("Action3"),
		"Injured_Reported");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + actionMargin + buttonSize,
		actionMargin,
		this->mpPanel->GetBitmapByName("Action4"),
		"Hole_In_Bulk");


	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + actionMargin + buttonSize,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("Action5"),
		"Ventilation_In");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + actionMargin + buttonSize,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("Action6"),
		"Ventilation_Out");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		actionMargin,
		this->mpPanel->GetBitmapByName("Action7"),
		"Cooling_Wall");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		(this->mMenuWidth / 2) - (buttonSize / 2),
		this->mpPanel->GetBitmapByName("Action8"),
		"Supporting_Wall");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("Action9"),
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

	//TODO ADD ACTION BUTTON OBSERVERS HERE
	this->mpPanel->GetButtonByName("Injured_Moved")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Treated")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Reported")->AddObserver(this);
	this->mpPanel->GetButtonByName("Hole_In_Bulk")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_In")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_Out")->AddObserver(this);
	this->mpPanel->GetButtonByName("Cooling_Wall")->AddObserver(this);
	this->mpPanel->GetButtonByName("Supporting_Wall")->AddObserver(this);
	this->mpPanel->GetButtonByName("Damaged_Bulk")->AddObserver(this);

}
