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
	Window *pWindow)
{
	this->mParentPanelWidth = parentWidth;
	this->mParentPanelHeight = parentHeight;

	this->mpEventLog = pEventLog;
	this->mpPanel = new Panel2D(
		this->mMenuWidth,									// Width
		this->mMenuHeight,									// Height
		this->mParentPanelHeight / 2 - this->mMenuHeight / 2,	// Top
		this->mParentPanelWidth / 2 - this->mMenuWidth / 2,		// Left
		pWindow->GetWindow(),
		windowName);
	
	this->InitButtons();

	return true;
}

bool EventMenu::Show(Room *pRoom, float screenX, float screenY)
{
	return false;
}

bool EventMenu::Update()
{
	this->mpPanel->Update();
	return false;
}

bool EventMenu::Draw() const
{
	if (this->mVisible)
		this->mpPanel->Draw();
	return true;
}

void EventMenu::Update(const Button & attribute)
{
	std::cout << "Clicked " << std::endl;
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
		"../../Models/Button04.png",
		"Fire");
	/*this->mpPanel->AddTextbox(
		100,
		30,

	)*/
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + buttonSize + padding,
		margin,
		"../../Models/Button03.png",
		"Water"); 
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin,
		margin + buttonSize + padding,
		"../../Models/Button02.png",
		"Gas");
	this->mpPanel->AddButton(
		buttonSize,
		buttonSize,
		margin + buttonSize + padding,
		margin + buttonSize + padding,
		"../../Models/Button01.png",
		"Injury");
	this->mpPanel->AddButton(
		this->mMenuWidth / 12,
		this->mMenuWidth / 12,
		0,
		this->mMenuWidth - this->mMenuWidth / 12,
		"../../Models/Exit.png",
		"Exit");
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);
	//this->mpPanel->GetButtonByName("Injury")->SetOpacity(0.6f);
}
