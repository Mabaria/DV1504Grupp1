#include "CrewPanel.h"
#include <fstream>

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
	//this->mSaveTextToDisk();
	this->mInitTimers();
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
	this->mpPanel->LoadImageToBitmap("../../Models/Off.png", "Off");
	this->mpPanel->LoadImageToBitmap("../../Models/On.png", "On");
	this->mpPanel->LoadImageToBitmap("../../Models/Grid.png", "Grid");
	this->mpPanel->LoadImageToBitmap("../../Models/RedSqr.png", "Red");

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
		this->mOpenWindow();
		this->mSaveMetaToDisk();
		this->mVisible = true;
		this->mpPanel->ShowOnTop();
	}
	else if (button->GetName() == "Crew" && this->mVisible == true)
	{
		this->mTimeData.sessionStart = 0;
		this->mSaveMetaToDisk();
		this->mVisible = false;
		this->mpPanel->Hide();
	}
	else if (button->GetName() == "Exit")
	{
		this->mTimeData.sessionStart = 0;
		this->mSaveMetaToDisk();
		this->mVisible = false;
		this->mpPanel->Hide();
	}
	else if (button->getBitmapPointer() ==
		this->mpPanel->GetBitmapByName("Off"))
	{
		// if button is off set it to on 
		button->SetBitmap(this->mpPanel->GetBitmapByName("On"));

		//make the button behind timestamp dr.disrespect(completely transparent)
		this->mpPanel->GetButtonByName(button->GetName() + "Text")->
			SetOpacity(0.0f);

		// change the bitmap of the timestamp button to dummy bitmap 
		this->mpPanel->GetButtonByName(button->GetName() + "Text")->
			SetBitmap(this->mpPanel->GetBitmapByName("Grid"));
		
		bool found = false;
		int i = 0;
		for (; i < 22; i++)
		{
			if (button->GetName() == this->mSeaMen[i])
			{
				found = true;
				break;
			}
		}
		Timer tempTimer;
		tempTimer.StartTimer();
		this->mTimeData.startTime[i] = tempTimer.GetTimeData();
		this->mTimeData.textState[i] = 0;
		this->mTimeData.pressed[i] = 1;
		this->mSaveMetaToDisk();
		this->mSaveTextToDisk();
		tempTimer.UpdateTimeStruct();
		// Set text with time
		if (button->GetName().back() != '§')
		{
			std::string temp = tempTimer.WhenTimerStarted();
			temp.insert(10, "\n\t\t\t\t  ");
			this->mpPanel->GetTextBoxByName(button->GetName())->SetText
			(button->GetName() + "\t\t\t\t  " + temp);
		}
		
	}
	else if (button->getBitmapPointer() ==
		this->mpPanel->GetBitmapByName("On"))
	{
		bool found = false;
		int i = 0;
		for (; i < 22; i++)
		{
			if (button->GetName() == this->mSeaMen[i])
			{
				found = true;
				break;
			}
		}
		this->mTimeData.pressed[i] = 0;
		this->mSaveMetaToDisk();
		this->mSaveTextToDisk();
		button->SetBitmap(this->mpPanel->GetBitmapByName("Off"));
		//this->mTimer.StartTimer();
		// Remove the time
		if (button->GetName().back() != '§')
		{
			this->mpPanel->GetTextBoxByName(button->GetName())->SetText
			(button->GetName());
		}		
	}
	// change the "skadad" icon
	else if (button->getBitmapPointer() ==
		this->mpPanel->GetBitmapByName("Red"))
	{
		bool found = false;
		int i = 0;
		for (; i < 22; i++)
		{
			if (button->GetName() == this->mSeaMen[i] + "§")
			{
				found = true;
				break;
			}
		}
		if (button->GetOpacity() == 0.0f)
		{
			button->SetOpacity(1.0f);
			this->mTimeData.hurt[i] = 1;
		}
		else
		{
			button->SetOpacity(0.0f);
			this->mTimeData.hurt[i] = 0;
		}
		this->mSaveMetaToDisk();
		this->mSaveTextToDisk();
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
	ID2D1Bitmap *tempBitmap;
	Timer tempTimer;

	int space = 3;
	int element_height = this->mpPanel->GetHeight() / 12 - space;

	for (int i = 0; i < 11; i++)
	{
		this->mpPanel->AddTextbox(
			this->mpPanel->GetWidth() / 2,
			element_height,
			(element_height + space) * (i + 1),
			5,
			this->mSeaMen[i],
			this->mSeaMen[i]);
		this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetFontSize
		(this->mFontSize);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			element_height,
			(element_height + space) * (i + 1),
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetBitmapByName("Off"),
			this->mSeaMen[i]);

		if (this->mTimeData.startTime[i] != 0)
		{
			tempTimer = Timer(this->mTimeData.startTime[i]);
			std::string temp = tempTimer.WhenTimerStarted();
			temp.insert(10, "\n\t\t\t\t  ");
			this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetText
			(this->mSeaMen[i] + "\t\t\t\t  " + temp);
		}

		if (this->mTimeData.textState[i] == 0)
		{
			tempBitmap = this->mpPanel->GetBitmapByName("Grid");
		}
		else
		{
			tempBitmap = this->mpPanel->GetBitmapByName("Red");
		}

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * (i + 1),
			this->mpPanel->GetWidth() / 8 * 2,
			tempBitmap,
			this->mSeaMen[i]+"Text");

		if (this->mTimeData.textState[i] == 2)
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")->
				SetOpacity(0.5f);
		}
		else
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")->
				SetOpacity(0.0f);
		}

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * (i + 1),
			this->mpPanel->GetWidth() / 8 * 3,
			this->mpPanel->GetBitmapByName("Red"),
			this->mSeaMen[i]+"§");
			//seamen name + § because they are on the same row so they need
			//the same "group name" 

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->AddObserver(this);

		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->AddObserver(this);

		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->SetOpacity(0.0f);

		//this->mpPanel->GetButtonByName(this->mSeaMen[i] +
		//	"Text")->SetOpacity(0.0f);

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetRenderWidth
		(this->mpPanel->GetBitmapByName("Off")->GetSize().width);

		this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->SetRenderWidth
		(this->mpPanel->GetBitmapByName("Red")->GetSize().width);
	}
	for (int i = 11; i < 22; i++)
	{
		this->mpPanel->AddTextbox(
			this->mpPanel->GetWidth() / 2,
			element_height,
			(element_height + space) * (i - 10),
			this->mpPanel->GetWidth() / 2 + 10,
			this->mSeaMen[i],
			this->mSeaMen[i]);
		this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetFontSize
		(this->mFontSize);

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			element_height,
			(element_height + space) * (i - 10),
			this->mpPanel->GetWidth() / 8 * 5,
			this->mpPanel->GetBitmapByName("Off"),
			this->mSeaMen[i]);

		if (this->mTimeData.startTime[i] != 0)
		{
			tempTimer = Timer(this->mTimeData.startTime[i]);
			std::string temp = tempTimer.WhenTimerStarted();
			temp.insert(10, "\n\t\t\t\t  ");
			this->mpPanel->GetTextBoxByName(this->mSeaMen[i])->SetText
			(this->mSeaMen[i] + "\t\t\t\t  " + temp);
		}

		ID2D1Bitmap *tempBitmap;
		if (this->mTimeData.textState[i] == 0)
		{
			tempBitmap = this->mpPanel->GetBitmapByName("Grid");
		}
		else
		{
			tempBitmap = this->mpPanel->GetBitmapByName("Red");
		}

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * ((i % 11) + 1),
			this->mpPanel->GetWidth() / 8 * 6,
			tempBitmap,
			this->mSeaMen[i] + "Text");

		if (this->mTimeData.textState[i] == 2)
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")->
				SetOpacity(0.5f);
		}
		else 
		{
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")->
				SetOpacity(0.0f);
		}

		this->mpPanel->AddButton(
			this->mpPanel->GetWidth() / 8,
			this->mpPanel->GetHeight() / 13 + 1,
			this->mpPanel->GetHeight() / 12 * ((i % 11) + 1),
			this->mpPanel->GetWidth() / 8 * 7,
			this->mpPanel->GetBitmapByName("Red"),
			this->mSeaMen[i] + "§");

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->AddObserver(this);

		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->AddObserver(this);

		this->mpPanel->GetButtonByName(this->mSeaMen[i] +
			"§")->SetOpacity(0.0f);

		//this->mpPanel->GetButtonByName(this->mSeaMen[i] +
		//	"Text")->SetOpacity(0.0f);

		this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetRenderWidth
		(this->mpPanel->GetBitmapByName("Off")->GetSize().width);

		this->mpPanel->GetButtonByName(this->mSeaMen[i]+"§")->SetRenderWidth
		(this->mpPanel->GetBitmapByName("Red")->GetSize().width);
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

void CrewPanel::mInitTimers()
{
	ZeroMemory(&this->mTimeData, sizeof(TimeData));
	std::fstream file("../../Savefiles/Metafiles/crew_log.meta", std::ios::in | std::ios::binary);
	if (!file.is_open())
		return;
	file.read((char*)&this->mTimeData, sizeof(TimeData));
	file.close();
}

void CrewPanel::mSaveMetaToDisk()
{

	std::fstream file("../../Savefiles/Metafiles/crew_log.meta",
		std::ios::out | std::ios::binary);
	if (!file.is_open())
		return;
	file.write((char*)&this->mTimeData, sizeof(TimeData));
	file.close();
}

void CrewPanel::mSaveTextToDisk()
{
	//this->mTimer = Timer();
	//std::string hello = this->mTimer.WhenTimerStarted();
	this->mSessionTimer.UpdateTimeStruct();
	std::fstream file(
		"../../Savefiles/Crewlog/crew_" + this->mSessionTimer.GetFileFriendlyString()
		+ ".txt",
		std::ios::out);
	if (!file.is_open())
		return;
	file << "Skeppsnummer\tNärvarande\t\tTid/Senast sedd\t\tSkadad" << std::endl
		<< "----------------------------------------------------------------"
		"-----------" << std::endl;
	for (int i = 0; i < 22; i++)
	{
		Timer tempTimer = Timer(this->mTimeData.startTime[i]);
		file << this->mSeaMen[i] << "\t\t"
			<< (this->mTimeData.pressed[i] ? "Ja" : "Nej") << "\t\t\t"
			<< (tempTimer.GetTimeData() != 0 ?
				tempTimer.WhenTimerStarted() : "\t\t") << "\t"
			<< (this->mTimeData.hurt[i] ? "Ja" : "Nej") << std::endl;
	}
		//<< "Mano\t\tNej\t\t\t" << this->mTimer.WhenTimerStarted()
		//<< "\tNej";
	file.close();
}

void CrewPanel::mOpenWindow()
{
	bool newSession = false;
	if (this->mTimeData.sessionStart == 0)
	{
		ZeroMemory(&this->mTimeData.pressed, sizeof(char) * 22);
		ZeroMemory(&this->mTimeData.hurt, sizeof(char) * 22);
		this->mSessionTimer = Timer();
		this->mTimeData.sessionStart = this->mSessionTimer.GetTimeData();
		newSession = true;
	}
	else 
	{
		this->mSessionTimer = Timer(this->mTimeData.sessionStart);
	}
	for (int i = 0; i < 22; i++)
	{
		// Change the bitmap from on to off
		if (this->mTimeData.pressed[i] == 0)
			this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetBitmap
			(this->mpPanel->GetBitmapByName("Off"));
		else 
			this->mpPanel->GetButtonByName(this->mSeaMen[i])->SetBitmap
			(this->mpPanel->GetBitmapByName("On"));

		// Sets the opacity of the "skadad" button to 0
		if (this->mTimeData.hurt[i] == 0)
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
				SetOpacity(0.0f);
		else
			this->mpPanel->GetButtonByName(this->mSeaMen[i] + "§")->
			SetOpacity(1.0f);

		if (newSession)
		{ 
			/*First time you clost the window the bitmap for the button behind
			the time stamp is changed*/
			if (this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")
				->getBitmapPointer() == this->mpPanel->GetBitmapByName("Grid"))
			{
				this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")
					->SetBitmap(this->mpPanel->GetBitmapByName("Red"));
				this->mTimeData.textState[i] = 1;
			}

			// The second time you clost the window you change the opacity
			// of the button
			else if (this->mpPanel->GetTextBoxByName
			(this->mSeaMen[i])->GetText().size() > 5 &&
				this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")
				->getBitmapPointer() == this->mpPanel->GetBitmapByName("Red"))
			{
				this->mpPanel->GetButtonByName(this->mSeaMen[i] + "Text")->
					SetOpacity(0.5f);
				this->mTimeData.textState[i] = 2;
			}
		}
	}
}
