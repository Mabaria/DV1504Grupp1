#include "NotificationObject.h"

NotificationObject::NotificationObject(
	Room * room, 
	LogEvent * event, 
	Direct2D *direct2d) 
	: mButton(direct2d, "", 0, 0, 0, 0)
	, mTextBox(direct2d, 0, 0, 0, 0)
{
	this->mTimer = event->GetTimer();
	this->mRoomName = room->GetName();
	this->mDeckName = room->GetDeckName();
	this->mStartTime = this->mTimer->WhenTimerStarted();
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
	this->mEventType = event->GetType();
	
	// 200x80 notification object.
	D2D1_RECT_F object_size;
	object_size.top = 0;
	object_size.left = 0;
	object_size.right = 200;
	object_size.bottom = 80;

	// 40x40 pixel icon.
	D2D1_RECT_F icon_size;
	icon_size.top = 20;
	icon_size.left = 10;
	icon_size.right = 50;
	icon_size.bottom = 60;

	// 150x80 text box.
	D2D1_RECT_F textbox_size;
	textbox_size.top = 0;
	textbox_size.left = 50;
	textbox_size.right = 190;
	textbox_size.bottom = 80;

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

	this->mButton.CreateButton(
		file_path, 
		object_size.left, 
		object_size.top, 
		object_size.right, 
		object_size.bottom);

	this->mButton.SetIconSize(
		icon_size.left,
		icon_size.top,
		icon_size.right,
		icon_size.bottom);

	this->mTextBox.SetTextBoxSize(
		textbox_size.left,
		textbox_size.top,
		textbox_size.right,
		textbox_size.bottom);

	this->mTextBox.SetText(this->GetNotificationString());
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
	std::string return_string = "";
	return_string += Event::GetString(this->mEventType);
	return_string += " ";
	return_string += this->mDeckName;
	return_string += " ";
	return_string += this->mRoomName;
	return_string += " ";
	return_string += this->mElapsedTime;

	return return_string;
}

const void NotificationObject::SetPosition(int x, int y)
{
	this->mButton.SetButtonsize(
		x, 
		y, 
		this->mButton.GetButtonsize().right + x, 
		this->mButton.GetButtonsize().bottom + y);
	
}

const void NotificationObject::Move(int x, int y)
{
	return void();
}

void NotificationObject::Update()
{
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
}
