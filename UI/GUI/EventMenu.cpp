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
	LPCTSTR windowName,
	HWND *pWindow)
{
	this->mParentPanelWidth = parentWidth;
	this->mParentPanelHeight = parentHeight;

	this->mMenuHeight = this->mParentPanelHeight;

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

	//! DET GÅR INTE ATT GÖMMA FÖNSTRET I BÖRJAN.
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
	if (this->mpActiveRoom)
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

		size = this->mpActiveRoom->GetActiveEventCount();

		std::vector<LogEvent*> active_events;
		this->mpActiveRoom->GetActiveEvents(active_events);

		for (int i = 0; i < size; i++)
		{
			switch (active_events[i]->GetType())
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
			case 73:
				this->mpPanel->GetButtonByName("Icon_Draining")->ForceButtState(BUTTON_STATE::CLICKED);
				break;
			case 74:
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
				obs_inf.actionData = Icon_Draining | Rotation_Stationary;
				this->NotifyObservers(&obs_inf);
			}
			else if (button_name.compare("Seal_Hole") == 0)
			{
				obs_inf.actionData = Icon_Seal_Hole | Rotation_Stationary;
				this->NotifyObservers(&obs_inf);
			}
			else if (button_name.compare("Damaged_Bulk") == 0)
			{
				obs_inf.actionData = Icon_Damaged_Bulk;
				this->NotifyObservers(&obs_inf);
			}
			else if (button_name.compare("Supporting_Wall") == 0)
			{
				obs_inf.actionData = Icon_Supporting_Wall;
				this->NotifyObservers(&obs_inf);
			}
		}
		else 
		{	//! ActionMode == NUMBERS
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

			static uint32_t numbers[] = {
				Number_1,
				Number_2,
				Number_3,
				Number_4,
				Number_5,
				Number_6,
				Number_7,
				Number_8,
				Number_9,
			};

			for (int i = 0; i < 9; i++)
			{
				if (button_name.compare("Number" + std::to_string(i + 1)) == 0)
				{
					obs_inf.actionData |= numbers[i];
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
	/* 
		-- Action Mode --
	
		0 = Standard
		1 =	Numbers

		The value can be either 0 or 1.
		This makes it possible to handle it as a bool.
	*/

	this->mActionMode =
		this->mActionMode != STANDARD ? STANDARD : NUMBERS;

	// Based on action mode, show or hide number buttons
	for (int i = 0; i < 9; i++)
	{
		this->mpPanel->GetButtonByName("Number" +
			std::to_string(i + 1))->SetAlive((int)this->mActionMode);
	}

	// Based on action mode, show or hide buttons other than number buttons
	int size = (int)this->mpPanel->GetNrOfButtons();
	for (int i = 0; i < size; i++)
	{
		Button* btn = this->mpPanel->GetButtonByIndex(i);
		if (btn->GetName().find("Number") == std::string::npos)
		{
			btn->SetAlive(!this->mActionMode);
		}
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
	int padding = this->mMenuWidth / 9;
	int actionMargin = this->mMenuWidth / 14;
	int buttonSize = this->mMenuWidth / 4 + 5;
	int topStartPos = this->mMenuHeight / 12 - 25;
	
	int left = actionMargin;
	int middle = (this->mMenuWidth / 2) - (buttonSize / 2);
	int right = this->mMenuWidth - buttonSize - actionMargin;
	int step = (buttonSize + actionMargin);
	// Event buttons

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos,
		left,
		this->mpPanel->GetBitmapByName("FireOff"),
		"Fire");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos,
		middle,
		this->mpPanel->GetBitmapByName("Action7"),
		"Cooling_Wall");



	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 1,
		left,
		this->mpPanel->GetBitmapByName("WaterOff"),
		"Water"); 

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 1,
		middle,
		this->mpPanel->GetBitmapByName("Action10"),
		"Icon_Draining");



	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 2,
		left,
		this->mpPanel->GetBitmapByName("GasOff"),
		"Gas");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 2,
		middle,
		this->mpPanel->GetBitmapByName("Action5"),
		"Ventilation_In");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 2,
		right,
		this->mpPanel->GetBitmapByName("Action6"),
		"Ventilation_Out");


	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 4,
		left,
		this->mpPanel->GetBitmapByName("Action9"),
		"Damaged_Bulk");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 4,
		middle,
		this->mpPanel->GetBitmapByName("Action8"),
		"Supporting_Wall");


	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 5,
		left,
		this->mpPanel->GetBitmapByName("Action4"),
		"Hole_In_Bulk");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 5,
		middle,
		this->mpPanel->GetBitmapByName("Action11"),
		"Seal_Hole");

	// Action buttons
	// Ordered:
	// 1 2 3
	// 4 5 6
	// 7 8 9

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 3,
		right,
		this->mpPanel->GetBitmapByName("Action1"),
		"Injured_Moved");

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 3,
		middle,
		this->mpPanel->GetBitmapByName("Action2"),
		"Injured_Treated");
		

	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		topStartPos + step * 3,
		left,
		this->mpPanel->GetBitmapByName("Action3"),
		"Injured_Reported");		

	

	// NUMBER BUTTONS
	for (int i = 0; i < 3; i++)
	{
		this->mpPanel->AddButton(
			buttonSize,
			buttonSize,
			topStartPos + step * (i + 3),
			left,
			this->mpPanel->GetBitmapByName("Number" + std::to_string((i * 3) + 1)),
			"Number" + std::to_string((i * 3) + 1));

		this->mpPanel->AddButton(
			buttonSize,
			buttonSize,
			topStartPos + step * (i + 3),
			middle,
			this->mpPanel->GetBitmapByName("Number" + std::to_string((i * 3) + 2)),
			"Number" + std::to_string((i * 3) + 2));

		this->mpPanel->AddButton(
			buttonSize,
			buttonSize,
			topStartPos + step * (i + 3),
			right,
			this->mpPanel->GetBitmapByName("Number" + std::to_string((i * 3) + 3)),
			"Number" + std::to_string((i * 3) + 3));

		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 1))->SetAlive(false);
		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 2))->SetAlive(false);
		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 3))->SetAlive(false);

		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 1))->AddObserver(this);
		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 2))->AddObserver(this);
		this->mpPanel->GetButtonByName("Number" +
			std::to_string((i * 3) + 3))->AddObserver(this);
	}


	this->mpPanel->GetButtonByName("Fire")->AddObserver(this);
	this->mpPanel->GetButtonByName("Gas")->AddObserver(this);
	this->mpPanel->GetButtonByName("Water")->AddObserver(this);
	
	this->mpPanel->GetButtonByName("Injured_Moved")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Treated")->AddObserver(this);
	this->mpPanel->GetButtonByName("Injured_Reported")->AddObserver(this);
	this->mpPanel->GetButtonByName("Hole_In_Bulk")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_In")->AddObserver(this);
	this->mpPanel->GetButtonByName("Ventilation_Out")->AddObserver(this);
	this->mpPanel->GetButtonByName("Cooling_Wall")->AddObserver(this);
	this->mpPanel->GetButtonByName("Icon_Draining")->AddObserver(this);
	this->mpPanel->GetButtonByName("Seal_Hole")->AddObserver(this);
	this->mpPanel->GetButtonByName("Damaged_Bulk")->AddObserver(this);
	this->mpPanel->GetButtonByName("Supporting_Wall")->AddObserver(this);
}
