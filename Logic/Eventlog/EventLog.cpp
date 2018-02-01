#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
}



/**
*	Log specific
*/

void EventLog::SetActiveLog(ActiveLog *pActiveLog)
{
	this->mpActiveLog = pActiveLog;
}



/**
*	Event specific
*/

// Returns index in active log
int EventLog::AddLogEvent(Event::Type type, int roomIndex)
{
	// Create an event to fill and push into the list
	LogEvent newEvent;

	int eventIndex = (int)this->mLogEvents.size();

	newEvent.SetActiveEventIndex(this->mpActiveLog->AddEvent(eventIndex, roomIndex));
	newEvent.SetType(type);

	this->mLogEvents.insert(this->mLogEvents.begin() + eventIndex, newEvent);
	this->SaveToFile("eventlog.txt");

	return newEvent.GetActiveEventIndex();
}

std::vector<Event::Type> EventLog::GetEvents(int activeEventIndex)
{
	
}



/**
*	Disk specific
*/

void EventLog::SaveToFile(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < this->mLogEvents.size(); i++)
	{
		file << "EVENT TEXT" << "\n";
	}

	file.close();
}

void EventLog::LoadFromFile(std::string filePath)
{
}
