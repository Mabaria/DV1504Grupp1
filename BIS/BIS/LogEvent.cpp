#include "LogEvent.h"

LogEvent::LogEvent()
{

}

LogEvent::LogEvent(Event::Type type, int roomEventIndex)
{
	this->mType = type;
	this->mRoomEventIndex = roomEventIndex;
}

LogEvent::~LogEvent()
{

}

//const TimeObject LogEvent::getStartTime() const;
//const TimeObject LogEvent::getElapsedTime() const;

void LogEvent::SetRoomEventIndex(int index)
{
	this->mRoomEventIndex = index;
}

void LogEvent::SetType(Event::Type type)
{
	this->mType = type;
}

int LogEvent::GetRoomEventIndex() const
{
	return this->mRoomEventIndex;
}

Event::Type LogEvent::GetType() const
{
	return this->mType;
}
