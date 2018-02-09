#include "NotificationObject.h"

NotificationObject::NotificationObject(Room * room, LogEvent * event, Direct2D *direct2d) : mButton(direct2d, "", 0, 0, 0, 0), mTextBox(direct2d, 0, 0, 0, 0)
{
	this->mTimer = event->GetTimer();
	this->mRoomName = room->GetName();
	this->mDeckName = room->GetDeckName();
	this->mStartTime = this->mTimer->WhenTimerStarted();
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
	this->mEventType = event->GetType();

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
	this->mButton.CreateButton(file_path, 0, 0, 0, 0);
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

void NotificationObject::Update()
{
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
}
