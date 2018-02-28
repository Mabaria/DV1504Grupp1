#include "CrewPanel.h"

CrewPanel::CrewPanel()
{
	this->mpPanel = nullptr;
	this->mSeaMen = new std::string[22] {
		"FC", 
		"S", 
		"FI",
		"Mtjc",
		"ISL",
		"1m",
		"2:m",
		"Slo",
		"Vapo",
		"Sigo",
		"Mano", 
		"Navo",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10"};
}

CrewPanel::~CrewPanel()
{
	delete this->mpPanel;
	delete this->mSeaMen;
}

void CrewPanel::Init(int width, int height, int top, int left, HWND parent, LPCTSTR title)
{
	this->mpPanel = new Panel2D(
		width,
		height,
		top,
		left,
		parent,
		title);

	this->mpPanel->Hide();
	this->mVisible = false;
	this->mpPanel->LoadImageToBitmap("../../Models/Exit.png", "Exit");
	this->mpPanel->AddButton(30, 30, 0, this->mpPanel->GetWidth() - 30,
		this->mpPanel->GetBitmapByName("Exit"), "Exit");
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);

	// Creates the title textbox.
	this->mpPanel->AddTextbox(
		this->mpPanel->GetWidth(),
		this->mpPanel->GetHeight() / 22,
		0,
		0,
		"Skeppsnummer		Närvaro/Saknas		Tid			Skadad",
		"title");

	this->mpPanel->GetTextBoxByName("title")->
		SetFontSize(22);
	this->mpPanel->GetTextBoxByName("title")->
		SetFontWeight(DWRITE_FONT_WEIGHT_ULTRA_BLACK);
}

void CrewPanel::Update()
{
	
}

void CrewPanel::Update(Button * button)
{
	if (button->GetName() == "Crew")
	{
		this->mVisible = true;
		this->mpPanel->ShowOnTop();
	}
	else if (button->GetName() == "Exit")
	{
		this->mVisible = false;
		this->mpPanel->Hide();
	}
}

void CrewPanel::Draw()
{
	if (this->mVisible)
	{
		this->mpPanel->Draw();
	}
	else
	{
		this->mpPanel->Hide();
	}
}

bool CrewPanel::IsMouseInsidePanel()
{
	return this->mpPanel->IsMouseInsidePanel();
}

void CrewPanel::mCreateTextBoxesAndButtons()
{
	for (int i = 0; i < this->mSeaMen->size(); i++)
	{
		this->mpPanel->AddTextbox(
			this->mpPanel->GetWidth(),
			this->mpPanel->GetHeight() / 22,
			this->mpPanel->GetHeight() / 22 * (i + 1),
			0,
			this->mSeaMen[i],
			this->mSeaMen[i]);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 4,
			this->mpPanel->GetHeight() / 22,
			this->mpPanel->GetHeight() / 22 * (i + 1),
			this->mpPanel->GetWidth() / 4 * 1,
			"gimmeRED",
			this->mSeaMen[i]);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 4,
			this->mpPanel->GetHeight() / 22,
			this->mpPanel->GetHeight() / 22 * (i + 1),
			this->mpPanel->GetWidth() / 4 * 3,
			"gimmeRED",
			this->mSeaMen[i]+"2");
		this->mpPanel->GetButtonByName(this->mSeaMen[i]+"2").
	}
}
