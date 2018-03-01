#include "LogAction.h"

LogAction::LogAction()
{
	this->mActive = true;
}

LogAction::LogAction(
	LogAction::Type type,
	std::string roomName) /*, // FIXME
	Actions *pActions,
	ActionPointer *pActionPointer)*/
{
	this->mActive = true;
	this->mType = type;
	this->mRoomName = roomName;
	//this->mpActions = pActions;
	//this->mpActionPointer = pActionsPointer;
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

//ActionPointer LogAction::GetActionsPointer() const // FIXME
//{
//	return this->mpActionPointer;
//}



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
