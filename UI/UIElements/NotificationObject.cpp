#include "NotificationObject.h"

NotificationObject::NotificationObject(Room * room, LogEvent * event)
{
	this->mTimer = event->GetTimer();
	this->mRoomName = room->GetName();
	this->mDeckName = room->GetDeckName();
	this->mStartTime = this->mTimer->WhenTimerStarted();
	this->mElapsedTime = this->mTimer->GetTimeAsStr();
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
