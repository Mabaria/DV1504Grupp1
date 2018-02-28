#include "Action.h"

Action::Action()
{
	
}

Action::Action(ActionType type, std::string roomName)
{
	this->mType = type;
	this->mRoomName = roomName;
}

Action::~Action()
{
	
}

// Action specific
void Action::SetType(ActionType type)
{
	this->mType = type;
}

ActionType Action::GetType() const
{
	return this->mType;
}

// Room specific
void Action::SetRoomName(std::string name)
{
	this->mRoomName = name;
}

std::string Action::GetRoomName() const
{
	return this->mRoomName;
}

// Time specific
const int Action::GetElapsedTime()
{
	return this->mTimer.GetSeconds();
}

const std::string Action::GetStartTimeAsString()
{
	return this->mTimer.WhenTimerStarted();
}

const std::string Action::GetElapsedTimeAsString()
{
	return this->mTimer.GetTimeAsStr();
}

Timestamp Action::GetTimestamp_Start() const
{
	return this->mTimer.GetTimestamp();
}
