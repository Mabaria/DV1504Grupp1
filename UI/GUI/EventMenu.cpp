#include "EventMenu.h"
#include <iostream>

EventMenu::EventMenu()
{
	this->mMenuWidth = 300;
	this->mMenuHeight = 300;
	this->mParentPanelWidth = 0;
	this->mParentPanelHeight = 0;

	this->mVisible = false;
	this->mpEventLog = nullptr;
	this->mpPanel = nullptr;
	this->mpActiveRoom = nullptr;
}

EventMenu::~EventMenu()
{
	if (this->mpPanel != nullptr)
		delete this->mpPanel;
}

bool EventMenu::Init(float parentWidth,
	float parentHeight,
	EventLog *pEventLog,
	LPCTSTR windowName,
	HWND *pWindow)
{
	this->mParentPanelWidth = parentWidth;
	this->mParentPanelHeight = parentHeight;

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
	
	this->InitButtons();

	this->mpPanel->Hide();

	return true;
}

bool EventMenu::OpenAt(Room *pRoom)
{
	int margin = this->mParentPanelWidth / 24;
	int posX = 0;
	int posY = 0;
	Position mousePos = Mouse::GetPositionPercentage();
	mousePos.x = mousePos.x * this->mParentPanelWidth;
	mousePos.y = mousePos.y * this->mParentPanelHeight;

	if (mousePos.x + margin + this->mMenuWidth < this->mParentPanelWidth)
		posX = mousePos.x + margin;
	else
		posX = mousePos.x - margin - this->mMenuWidth;

	if (mousePos.y + margin + this->mMenuHeight < this->mParentPanelHeight)
		posY = mousePos.y + margin;
	else
		posY = mousePos.y - margin - this->mMenuHeight;

	this->mpPanel->SetLeft(posX);
	this->mpPanel->SetTop(posY);
	this->mpPanel->Show();
	this->mpPanel->UpdateWindowPos();

	this->mpActiveRoom = pRoom;

	return true;
}

bool EventMenu::Update()
{
	this->mpPanel->Update();
	return false;
}

bool EventMenu::Draw() const
{
	this->mpPanel->Draw();
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
	int buttonSize = this->mMenuWidth / 4;
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
	//this->mpPanel->GetButtonByName("Injury")->SetOpacity(0.6f);
}
