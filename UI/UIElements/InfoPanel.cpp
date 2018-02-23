#include "InfoPanel.h"

InfoPanel::InfoPanel()
{
	this->mpPanel = nullptr;

}

InfoPanel::~InfoPanel()
{
	delete this->mpPanel;
}

void InfoPanel::Init(int width, int height, int top, int left, HWND parent, LPCTSTR title)
{
	RECT client_size;
	GetClientRect(parent, &client_size);
	this->mParentPanelHeight = client_size.bottom - client_size.top;
	this->mParentPanelWidth  = client_size.right - client_size.left;

	this->mpPanel = new Panel2D(
		width / 3 + 10,
		height - 100,
		10,
		width / 3 + 10,
		parent,
		title);

	this->mpPanel->Hide();

	this->mpPanel->LoadImageToBitmap("../../Models/Exit.png", "Exit");
	this->mpPanel->AddButton(30, 30, 0, this->mpPanel->GetWidth() - 30,
		this->mpPanel->GetBitmapByName("Exit"), "Exit");
	this->mpPanel->GetButtonByName("Exit")->AddObserver(this);

	//! TEXT STUFF
	std::string title_string = "Anv�ndarguide";
	std::vector<std::string> header_strings;
	std::vector<std::string> body_strings;

	int title_font_size		= 40;
	int header_font_size	= 30;
	int body_font_size		= 20;

	// Multiply these by number of rows of text for text box height.
	int title_textbox_height	= title_font_size  * 4 / 3 + 5;
	int header_textbox_height	= header_font_size * 4 / 3 + 5;
	int body_textbox_height		= body_font_size   * 4 / 3 + 5;

	// For placing the textboxes.
	int info_height = 0;

	header_strings.push_back("Kontroller");
	body_strings.push_back("V�nster musknapp: V�lja rum eller h�ndelser "
		"samt l�gga till och ta bort h�ndelser.\n"
		"H�ger musknapp: H�ll in och dra f�r att rotera kameran.");

	this->mpPanel->AddTextbox(
		this->mpPanel->GetWidth(),
		title_textbox_height,
		info_height,
		0,
		title_string,
		"title");

	this->mpPanel->GetTextBoxByName("title")->
		SetFontSize(title_font_size);
	this->mpPanel->GetTextBoxByName("title")->
		SetFontWeight(DWRITE_FONT_WEIGHT_ULTRA_BLACK);
	this->mpPanel->GetTextBoxByName("title")->
		SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	this->mpPanel->GetTextBoxByName("title")->
		SetFontName(L"comic sans ms");

	info_height += title_textbox_height;

	this->mpPanel->AddTextbox(
		this->mpPanel->GetWidth(),
		header_textbox_height,
		info_height,
		0,
		header_strings[0],
		"controls_header");

	this->mpPanel->GetTextBoxByName("controls_header")->
		SetFontSize(header_font_size);
	this->mpPanel->GetTextBoxByName("controls_header")->
		SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	this->mpPanel->GetTextBoxByName("controls_header")->
		SetFontName(L"comic sans ms");

	info_height += header_textbox_height;

	this->mpPanel->AddTextbox(
		this->mpPanel->GetWidth(),
		body_textbox_height * 3,
		info_height,
		0,
		body_strings[0],
		"controls_body");
	this->mpPanel->GetTextBoxByName("controls_body")->
		SetFontSize(body_font_size);
	this->mpPanel->GetTextBoxByName("controls_body")->
		SetFontName(L"comic sans ms");

	info_height += body_textbox_height * 3;
}

void InfoPanel::Update()
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

	if (this->mDraggingWindow)
	{
		int left = Mouse::GetExactX() - this->mDragX;
		if (left + this->mpPanel->GetWidth() > this->mParentPanelWidth)
			left = this->mParentPanelWidth - this->mpPanel->GetWidth();
		else if (left < 0)
			left = 0;

		int top = Mouse::GetExactY() - this->mDragY;
		if (top + this->mpPanel->GetHeight() > this->mParentPanelHeight)
			top = this->mParentPanelHeight - this->mpPanel->GetHeight();
		else if (top < 0)
			top = 0;

		this->mpPanel->SetLeft(left);
		this->mpPanel->SetTop(top);
		this->mpPanel->UpdateWindowPos();
	}
}

void InfoPanel::Update(Button *button)
{

	this->mVisible = true;
	this->mpPanel->ShowOnTop();
}

void InfoPanel::Draw()
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
