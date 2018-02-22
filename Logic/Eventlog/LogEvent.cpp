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

std::string LogEvent::GetFileString()
{
	/**
	*	Will return a one-line string to be printed to the .log file. Ex:
	*	Jan 23 12:17:03 Eldsvåda startade i Maskinrum
	*/

	std::string print = "";

	print += this->mTimer.WhenTimerStarted();

	return print;
}



/**
*	Time specific
*/

void LogEvent::StartTimer()
{
	this->mTimer.StartTimer();
}

const int LogEvent::GetElapsedTime()
{
	return this->mTimer.GetSeconds();
}

const std::string LogEvent::GetStartTimeAsString()
{
	return this->mTimer.WhenTimerStarted();
}

const std::string LogEvent::GetElapsedTimeAsString()
{
	return this->mTimer.GetTimeAsStr();
}

Timer * LogEvent::GetTimer()
{
	return &this->mTimer;
}




