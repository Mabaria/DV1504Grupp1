#include "LogAction.h"

LogAction::LogAction()
{
	this->mActive = true;
}

LogAction::LogAction(LogAction::Desc desc)
{
	this->mActive = true;

	this->mType = desc.type;
	this->mRoomName = desc.roomName;
	this->mActionIndex = desc.actionIndex;
	this->mPos_x = desc.pos_x;
	this->mPos_z = desc.pos_z;
}

LogAction::~LogAction()
{
	
}



/**
*	Action specific
*/

void LogAction::SetInactive()
{
	this->mActive = false;
}

void LogAction::SetType(LogAction::Type type)
{
	this->mType = type;
}

bool LogAction::IsActive() const
{
	return this->mActive;
}

LogAction::Type LogAction::GetType() const
{
	return this->mType;
}

int LogAction::GetActionIndex() const
{
	return this->mActionIndex;
}



/**
*	Room specific
*/

void LogAction::SetRoomName(std::string name)
{
	this->mRoomName = name;
}

std::string LogAction::GetRoomName() const
{
	return this->mRoomName;
}



/**
*	Time specific
*/

const int LogAction::GetElapsedTime()
{
	return this->mTimer.GetSeconds();
}

const std::string LogAction::GetStartTimeAsString()
{
	return this->mTimer.WhenTimerStarted();
}

const std::string LogAction::GetElapsedTimeAsString()
{
	return this->mTimer.GetTimeAsStr();
}

Timestamp LogAction::GetTimestamp_Start() const
{
	return this->mTimer.GetTimestamp();
}
