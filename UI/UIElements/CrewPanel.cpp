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
	this->mFontSize = 26;
}

CrewPanel::~CrewPanel()
{
	delete this->mpPanel;
	delete[] this->mSeaMen;
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
	this->mpPanel->LoadImageToBitmap("../../Models/RedSqr.png", "Red");
	this->mpPanel->LoadImageToBitmap("../../Models/GreenSqr.png", "Green");
	this->mpPanel->LoadImageToBitmap("../../Models/Grid.png", "Grid");

	this->mpPanel->AddButton(50, 50, 0, this->mpPanel->GetWidth() - 50,
		this->mpPanel->GetBitmapByName("Exit"), "Exit");
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);

	// Creates the title textbox.
	this->mpPanel->AddTextbox(
		this->mpPanel->GetWidth(),
		this->mpPanel->GetHeight() / 22,
		10,
		0,
		"Skeppsnummer	  Närvaro/Saknas               Tid		   Skadad\t   " 
		"           Skeppsnummer      Närvaro/Saknas	      Tid		Skadad",
		"title");

	this->mpPanel->GetTextBoxByName("title")->
		SetFontSize(26);
	this->mpPanel->GetTextBoxByName("title")->
		SetFontWeight(DWRITE_FONT_WEIGHT_ULTRA_BLACK);
	this->mCreateTextBoxesAndButtons();
}

void CrewPanel::Update()
{
	this->mpPanel->Update();
}

void CrewPanel::Update(Button * button)
{
	if (button->GetName() == "Crew" && this->mVisible == false)
	{
		this->mVisible = true;
		this->mpPanel->ShowOnTop();
	}
	else if (button->GetName() == "Crew" && this->mVisible == true)
	{
		for (int i = 0; i < 22; i++)
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetBitmap
			(this->mpPanel->GetBitmapByName("Red"));
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
				SetOpacity(0.0f);
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
				SetBitmap(this->mpPanel->GetBitmapByName("Green"));
		}
		this->mVisible = false;
		this->mpPanel->Hide();
	}
	else if (button->GetName() == "Exit")
	{
		for (int i = 0; i < 22; i++)
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetBitmap
			(this->mpPanel->GetBitmapByName("Red"));
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
				SetOpacity(0.0f);
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
				SetBitmap(this->mpPanel->GetBitmapByName("Green"));
		}
		this->mVisible = false;
		this->mpPanel->Hide();
	}
	else if (button->getBitmapPointer() ==
		this->mpPanel->GetBitmapByName("Red"))
	{
		button->SetBitmap(this->mpPanel->GetBitmapByName("Green"));
		if (button->GetName().back()=='§')
		{
			button->SetOpacity(0.0f);
		}
		this->mtimer.StartTimer();
		if (button->GetName().back() != '§')
		{
			std::string temp = this->mtimer.WhenTimerStarted();
			temp.insert(10, "\n\t\t\t\t  ");
			this->mpPanel->GetTextBoxByName(button->GetName())->SetText
			(button->GetName() + "\t\t\t\t  " + temp);
		}
		
	}
	else if (button->getBitmapPointer() ==
		this->mpPanel->GetBitmapByName("Green"))
	{
		button->SetOpacity(1.0f);
		button->SetBitmap(this->mpPanel->GetBitmapByName("Red"));
		this->mtimer.StartTimer();
		if (button->GetName().back() != '§')
		{
			this->mpPanel->GetTextBoxByName(button->GetName())->SetText
			(button->GetName());
		}
		
		
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
	for (int i = 0; i < 11; i++)
	{
		this->mpPanel->AddTextbox(
			this->mpPanel->GetWidth() / 2,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * (i + 1),
			5,
			this->mSeaMen[i],
			this->mSeaMen[i]);
		this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetFontSize
		(this->mFontSize);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * (i + 1),
			this->mpPanel->GetWidth() / 8 * 1,
			this->mpPanel->GetBitmapByName("Red"),
			this->mSeaMen[i]);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * (i + 1),
			this->mpPanel->GetWidth() / 8 * 3,
			this->mpPanel->GetBitmapByName("Green"),
			this->mSeaMen[i]+"§");

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->AddObserver(this);
		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->AddObserver(this);
		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->SetOpacity(0.0f);
	}
	for (int i = 11; i < 22; i++)
	{
		this->mpPanel->AddTextbox(
			this->mpPanel->GetWidth() / 2,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * ((i % 11) + 1),
			this->mpPanel->GetWidth() / 2 + 10,
			this->mSeaMen[i],
			this->mSeaMen[i]);
		this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetFontSize
		(this->mFontSize);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * ((i % 11) + 1),
			this->mpPanel->GetWidth() / 8 * 5,
			this->mpPanel->GetBitmapByName("Red"),
			this->mSeaMen[i]);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * ((i % 11) + 1),
			this->mpPanel->GetWidth() / 8 * 7,
			this->mpPanel->GetBitmapByName("Green"),
			this->mSeaMen[i] + "§");

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->AddObserver(this);
		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->AddObserver(this);
		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->SetOpacity(0.0f);
	}
	this->mpPanel->AddButton(
		this->mpPanel->GetWidth(),
		this->mpPanel->GetHeight(),
		0,
		0,
		this->mpPanel->GetBitmapByName("Grid"), "Grid");
	this->mpPanel->GetButtonByName("Grid")->SetRenderWidth
	(this->mpPanel->GetBitmapByName("Grid")->GetSize().width);
}
