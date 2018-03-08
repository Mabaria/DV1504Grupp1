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
	this->mInjuryType = REPORTED;
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
	this->mpPanel->LoadImageToBitmap("../../Models/Action10.png", "Action10");
	this->mpPanel->LoadImageToBitmap("../../Models/Action11.png", "Action11");

	this->mpPanel->LoadImageToBitmap("../../Models/Number1.png", "Number1");
	this->mpPanel->LoadImageToBitmap("../../Models/Number2.png", "Number2");
	this->mpPanel->LoadImageToBitmap("../../Models/Number3.png", "Number3");
	this->mpPanel->LoadImageToBitmap("../../Models/Number4.png", "Number4");
	this->mpPanel->LoadImageToBitmap("../../Models/Number5.png", "Number5");
	this->mpPanel->LoadImageToBitmap("../../Models/Number6.png", "Number6");
	this->mpPanel->LoadImageToBitmap("../../Models/Number7.png", "Number7");
	this->mpPanel->LoadImageToBitmap("../../Models/Number8.png", "Number8");
	this->mpPanel->LoadImageToBitmap("../../Models/Number9.png", "Number9");
	
	this->InitButtons();

	//! DET G�R INTE ATT G�MMA F�NSTRET I B�RJAN.
	this->mpPanel->Hide();

	return true;
}

bool EventMenu::OpenAt(Room *pRoom)
{
	int margin = this->mParentPanelWidth / 24;
	int posX = this->mParentPanelWidth - this->mMenuWidth;
	int posY = 0;

	this->mpPanel->SetLeft(posX);
	this->mpPanel->SetTop(posY);
	this->mpPanel->UpdateWindowPos();
	this->mpPanel->Show();

	this->mVisible = true;

	this->mpActiveRoom = pRoom;

	this->UpdateEventButtonImages();

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

void EventMenu::Close()
{
	this->mVisible = false;
	this->mpPanel->Hide();
	if (this->mActionMode == NUMBERS)
		this->mSwapActionMode(); // Menu closing, reset buttons
}

bool EventMenu::IsMouseInsidePanel()
{
	return this->mpPanel->IsMouseInsidePanel();
}

bool EventMenu::IsVisible()
{
	return this->mpPanel->IsVisible();
}

void EventMenu::UpdateEventButtonImages()
{

	// Names of possible active events
	std::string data_names[] = { "Fire", "Water", "Gas" };

	// Deactivates all buttons (no event active)
	int size = (int)floor(sizeof(data_names) / sizeof(std::string));
	for (int i = 0; i < size; i++)
	{
		if (this->mpPanel->GetButtonByName(data_names[i]))
		{
			this->mpPanel->GetButtonByName(data_names[i])->SetBitmap(
				this->mpPanel->GetBitmapByName(data_names[i] + "Off"));
		}
	}

	// Activate corresponding buttons only if there are any active events 
	// (activate necessary events)
	if (this->mpActiveRoom->GetActiveEventIndex() != -1)
	{
		int size = (int)this->mpActiveRoom->GetActiveEvents().size();
		for (int i = 0; i < size; i++)
		{
			switch (this->mpActiveRoom->GetActiveEvents()[i]->GetType())
			{
			case Event::Type::Fire:
				this->mpPanel->GetButtonByName("Fire")->SetBitmap(
					this->mpPanel->GetBitmapByName("FireOn"));
				break;
			case Event::Type::Water:
				this->mpPanel->GetButtonByName("Water")->SetBitmap(
					this->mpPanel->GetBitmapByName("WaterOn"));
				break;
			case Event::Type::Gas:
				this->mpPanel->GetButtonByName("Gas")->SetBitmap(
					this->mpPanel->GetBitmapByName("GasOn"));
				break;
			}
		}
	}
}

void EventMenu::SetActiveRoom(Room * room)
{
	this->mpActiveRoom = room;
}

void EventMenu::Update(ActionHandler * attribute)
{
	ObserverInfo temp = attribute->GetLastEvent();
	// First clear all previously locked states
	this->mClearActionButtStates();
	// If the actionhandler is ready to place an event, find out which one and lock
	// that button in "clicked" state
	if (attribute->IsWaiting())
	{
		if (temp.actionData % 512 == 64) {
			this->mpPanel->GetButtonByName("Injured_Moved")->ForceButtState(BUTTON_STATE::CLICKED);
		}
		else if (temp.actionData % 512 == 65) {
			this->mpPanel->GetButtonByName("Injured_Treated")->ForceButtState(BUTTON_STATE::CLICKED);
		}
		else if (temp.actionData % 512 == 66) {
			this->mpPanel->GetButtonByName("Injured_Reported")->ForceButtState(BUTTON_STATE::CLICKED);
		}
		else
		{
			switch (temp.actionData) {
			case 3:
				this->mpPanel->GetButtonByName("Hole_In_Bulk")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 4:
				this->mpPanel->GetButtonByName("Ventilation_In")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 5:
				this->mpPanel->GetButtonByName("Ventilation_Out")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 6:
				this->mpPanel->GetButtonByName("Cooling_Wall")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 7:
				this->mpPanel->GetButtonByName("Supporting_Wall")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 8:
				this->mpPanel->GetButtonByName("Damaged_Bulk")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 9:
				this->mpPanel->GetButtonByName("Icon_Draining")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 10:
				this->mpPanel->GetButtonByName("Seal_Hole")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			}
		}
	}
}

void EventMenu::Update(Button *attribute)
{
	if (this->mpPanel->IsVisible())
	{
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
			ObserverInfo obs_inf;
			obs_inf.pRoom = this->mpActiveRoom;

			if (button_name.compare("Fire") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Fire);
				this->mLastClicked = Event::Fire;
				obs_inf.actionData = No_Action;
				this->NotifyObservers(&obs_inf);
			}
			else if (button_name.compare("Gas") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Gas);
				this->mLastClicked = Event::Gas;
				obs_inf.actionData = No_Action;
				this->NotifyObservers(&obs_inf);
			}
			else if (button_name.compare("Water") == 0)
			{
				this->mpActiveRoom->AddPlotterEvent(Event::Water);
				this->mLastClicked = Event::Water;
				obs_inf.actionData = No_Action;
				this->NotifyObservers(&obs_inf);
			}
			else if (this->mActionMode == STANDARD)
			{

				if (button_name.compare("Injured_Moved") == 0)
				{
					this->mInjuryType = MOVED;
					this->mClearActionButtStates();
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Treated") == 0)
				{					
					this->mInjuryType = TREATED;
					this->mClearActionButtStates();
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Reported") == 0)
				{
					this->mInjuryType = REPORTED;
					this->mClearActionButtStates();
					this->mSwapActionMode();
				}
				else if (button_name.compare("Hole_In_Bulk") == 0)
				{
					obs_inf.actionData = Icon_Hole_In_Bulk;
					this->NotifyObservers(&obs_inf);
				}
				else if (button_name.compare("Ventilation_In") == 0)
				{
					obs_inf.actionData = Icon_Ventilation_In;
					this->NotifyObservers(&obs_inf);
				}
				else if (button_name.compare("Ventilation_Out") == 0)
				{
					obs_inf.actionData = Icon_Ventilation_Out;
					this->NotifyObservers(&obs_inf);
				}
				else if (button_name.compare("Cooling_Wall") == 0)
				{
					obs_inf.actionData = Icon_Cooling_Wall;
					this->NotifyObservers(&obs_inf);
				}
				else if (button_name.compare("Icon_Draining") == 0)
				{
					obs_inf.actionData = Icon_Draining;
					this->NotifyObservers(&obs_inf);
				}
				else if (button_name.compare("Seal_Hole") == 0)
				{
					obs_inf.actionData = Icon_Seal_Hole;
					this->NotifyObservers(&obs_inf);
				}
			}
			else { //! ActionMode == NUMBERS
				// Injury action has been selected, and these buttons now
				// correspond to the amount of injured people to draw the symbol
				// with, from 1-9
				//! this->mInjuryType contains which kind of triangle to use
				//! MOVED, TREATED or REPORTED
				switch (this->mInjuryType)
				{
				case MOVED:
					obs_inf.actionData = Icon_Injured_Moved | Rotation_Stationary;
					break;
				case REPORTED:
					obs_inf.actionData = Icon_Injured_Reported | Rotation_Stationary;
					break;
				case TREATED:
					obs_inf.actionData = Icon_Injured_Treated | Rotation_Stationary;
					break;
				}
				if (button_name.compare("Injured_Moved") == 0) //! 1
				{
					obs_inf.actionData |= Number_1;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Treated") == 0) //! 2
				{
					obs_inf.actionData |= Number_2;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Injured_Reported") == 0) //! 3
				{
					obs_inf.actionData |= Number_3;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Hole_In_Bulk") == 0) //! 4
				{
					obs_inf.actionData |= Number_4;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Ventilation_In") == 0) //! 5
				{
					obs_inf.actionData |= Number_5;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Ventilation_Out") == 0) //! 6
				{
					obs_inf.actionData |= Number_6;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Cooling_Wall") == 0) //! 7
				{
					obs_inf.actionData |= Number_7;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Icon_Draining") == 0) //! 8
				{
					obs_inf.actionData |= Number_8;
					this->NotifyObservers(&obs_inf);
					this->mSwapActionMode();
				}
				else if (button_name.compare("Seal_Hole") == 0) //! 9
				{
					obs_inf.actionData |= Number_9;
					this->NotifyObservers(&obs_inf);
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
			this->mpPanel->GetBitmapByName("Number1"));

		this->mpPanel->GetButtonByName("Injured_Treated")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number2"));

		this->mpPanel->GetButtonByName("Injured_Reported")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number3"));

		this->mpPanel->GetButtonByName("Hole_In_Bulk")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number4"));

		this->mpPanel->GetButtonByName("Ventilation_In")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number5"));

		this->mpPanel->GetButtonByName("Ventilation_Out")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number6"));

		this->mpPanel->GetButtonByName("Cooling_Wall")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number7"));

		this->mpPanel->GetButtonByName("Icon_Draining")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number8"));

		this->mpPanel->GetButtonByName("Seal_Hole")->SetBitmap(
			this->mpPanel->GetBitmapByName("Number9"));
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

		this->mpPanel->GetButtonByName("Icon_Draining")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action10"));

		this->mpPanel->GetButtonByName("Seal_Hole")->SetBitmap(
			this->mpPanel->GetBitmapByName("Action11"));
	}
}

void EventMenu::mClearActionButtStates()
{
	Button * temp;
	// Nullptr checks before clearing, in case a named button doesnt exist
	// This should probably exist in more places in this class, because
	// debugging nullptr function calls from GetButtonByName is gay af
	temp = this->mpPanel->GetButtonByName("Injured_Moved");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Injured_Treated");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Injured_Reported");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Hole_In_Bulk");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Ventilation_In");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Ventilation_Out");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Cooling_Wall");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Supporting_Wall");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Damaged_Bulk");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Icon_Draining");
	if (temp)
		temp->SetForcedButtState(false);

	temp = this->mpPanel->GetButtonByName("Seal_Hole");
	if (temp)
		temp->SetForcedButtState(false);
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
		this->mMenuWidth / 2 - eventButtonSize / 2,
		this->mpPanel->GetBitmapByName("WaterOff"),
		"Water"); 
	this->mpPanel->AddButton(
		eventButtonSize,
		eventButtonSize,
		margin,
		this->mMenuWidth - margin - eventButtonSize,
		this->mpPanel->GetBitmapByName("GasOff"),
		"Gas");

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
		this->mpPanel->GetBitmapByName("Action10"),
		"Icon_Draining");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + (eventButtonSize + margin) * 2 + (actionMargin + buttonSize) * 2,
		this->mMenuWidth - buttonSize - actionMargin,
		this->mpPanel->GetBitmapByName("Action11"),
		"Seal_Hole");

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
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);

	//TODO ADD ACTION BUTTON OBSERVERS HERE
	this->mpPanel->GetButtonByName("Injured_Moved")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Treated")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Reported")->AddObserver(this);
	this->mpPanel->GetButtonByName("Hole_In_Bulk")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_In")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_Out")->AddObserver(this);
	this->mpPanel->GetButtonByName("Cooling_Wall")->AddObserver(this);
	this->mpPanel->GetButtonByName("Icon_Draining")->AddObserver(this);
	this->mpPanel->GetButtonByName("Seal_Hole")->AddObserver(this);

}
