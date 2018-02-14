#include "NotificationObject.h"

NotificationObject::NotificationObject(
	Room *room, 
	LogEvent *event, 
	Direct2D *direct2d,
	int index) 
	: mButton(direct2d, "", 0, 0, 0, 0)
	, mTextBox(direct2d, 0, 0, 0, 0)
{
	this->mTimer = event->GetTimer();
	this->mRoomName = room->GetName();
	this->mDeckName = room->GetDeckName();
	this->mStartTime = this->mTimer->WhenTimerStarted();
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
	this->mEventType = event->GetType();
	this->mIndex = index;
	
	this->mTextBox.SetFontSize(12);
	this->mTextBox.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	D2D1_SIZE_F render_target_size = direct2d->GetpRenderTarget()->GetSize();	

	// 200x80 notification object.
	D2D1_RECT_F object_size;
	object_size.top = 0;
	object_size.left = 0;
	object_size.right = render_target_size.width - 4;
	object_size.bottom = render_target_size.height / 11;

	// 40x40 pixel icon.
	D2D1_RECT_F icon_size;
	icon_size.top = 5;
	icon_size.left = 5;
	icon_size.right = object_size.bottom - 5;
	icon_size.bottom = object_size.bottom - 5;

	// 150x80 text box.
	D2D1_RECT_F textbox_size;
	textbox_size.top = icon_size.top;
	textbox_size.left = icon_size.right + 5;
	textbox_size.right = 19 * render_target_size.width / 20;
	textbox_size.bottom = object_size.bottom;

	std::string file_path = "../../Models/";
	switch (this->mEventType)
	{
	case Event::Type::Fire:
		file_path += "Button04.png";
		break;
	case Event::Type::Water:
		file_path += "Button03.png";
		break;
	case Event::Type::Gas:
		file_path += "Button02.png";
		break;
	case Event::Type::Injury:
		file_path += "Button01.png";
		break;
	default:
		file_path += "pepehands.jpg";
	}

	// Every notification object starts at the same place,
	// the handler places them where they need to be.
	this->mButton.CreateButton(
		file_path, 
		(int)object_size.left,
		(int)object_size.top,
		(int)object_size.right,
		(int)object_size.bottom);

	this->mButton.SetIconSize(
		(int)icon_size.left,
		(int)icon_size.top,
		(int)icon_size.right,
		(int)icon_size.bottom);


	this->mTextBox.SetTextBoxSize(
		(int)textbox_size.left,
		(int)textbox_size.top,
		(int)textbox_size.right,
		(int)textbox_size.bottom);

	this->mTextBox.SetText(this->GetNotificationString());
	this->mButton.SetButtonStatus(BUTTON_STATE::IDLE);
}

NotificationObject::~NotificationObject()
{
}

const Event::Type NotificationObject::GetEventType() const
{
	return this->mEventType;
}

const std::string NotificationObject::GetRoomName() const
{
	return this->mRoomName;
}

const std::string NotificationObject::GetDeckName() const
{
	return this->mDeckName;
}

const std::string NotificationObject::GetElapsedTime() const
{
	return this->mElapsedTime;
}

const std::string NotificationObject::GetStartTime() const
{
	return this->mStartTime;
}

const std::string NotificationObject::GetNotificationString() const
{
	std::string return_string = "Händelse ";
	return_string += std::to_string(this->mIndex);
	return_string += "\n";
	return_string += this->mRoomName;
	return_string += "\n";
	return_string += this->mDeckName;
	return_string += "\n";
	return_string += this->mElapsedTime;
	
	return return_string;
}

Button * NotificationObject::GetButton()
{
	return &this->mButton;
}

const int NotificationObject::GetWidth() const
{
	return (int)this->mButton.GetButtonSize().right 
		-  (int)this->mButton.GetButtonSize().left;
}

const int NotificationObject::GetHeight() const
{
	return (int)this->mButton.GetButtonSize().bottom
		-  (int)this->mButton.GetButtonSize().top;
}

const int NotificationObject::GetIndex() const
{
	return this->mIndex;
}

const void NotificationObject::SetIndex(int index)
{
	this->mIndex = index;
}

const void NotificationObject::Move(int x, int y)
{
	this->mButton.MoveButton(x, y);
	this->mTextBox.MoveTextBox(x, y);
}

const void NotificationObject::SetText(std::string text)
{
	this->mTextBox.SetText(text);
}

void NotificationObject::Update()
{
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
	this->SetText(this->GetNotificationString());
}

void NotificationObject::Draw()
{
	this->mButton.DrawRect();
	this->mTextBox.DrawTextBox();
	this->mButton.DrawButton();
}
