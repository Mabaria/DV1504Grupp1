#include "LogEvent.h"

LogEvent::LogEvent()
{

}

LogEvent::LogEvent(Event::Type type, int roomEventIndex)
{
	this->mType = type;
	this->mActiveEventIndex = roomEventIndex;
}

LogEvent::~LogEvent()
{

}

//const TimeObject LogEvent::getStartTime() const;
//const TimeObject LogEvent::getElapsedTime() const;



/**
*	Event specific
*/

void LogEvent::SetType(Event::Type type)
{
	this->mType = type;
}

Event::Type LogEvent::GetType() const
{
	return this->mType;
}



/**
*	Log specific
*/

void LogEvent::SetActiveEventIndex(int index)
{
	this->mActiveEventIndex = index;
}

int LogEvent::GetActiveEventIndex() const
{
	return this->mActiveEventIndex;
}

const int LogEvent::GetElapsedTime()
{
	return this->mTime.GetSeconds();
}

const std::string LogEvent::GetStartTimeAsString()
{
	return this->mTime.WhenTimerStarted();
}

const std::string LogEvent::GetElapsedTimeAsString()
{
	return this->mTime.GetTimeAsStr();
}




