#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
}

int EventLog::AddLogEvent(Event::Type type, int roomIndex)
{
	LogEvent newEvent;
	int eventIndex;

	eventIndex = (int)this->mLogEvents.size();

	newEvent.SetRoomEventIndex(this->mpActiveLog->AddEvent(eventIndex, roomIndex));
	newEvent.SetType(type);

	this->mLogEvents.insert(this->mLogEvents.begin() + eventIndex, newEvent);
	this->SaveToFile("eventlog.txt");

	return newEvent.GetRoomEventIndex();
}

void EventLog::SetActiveLog(ActiveLog *pActiveLog)
{
	this->mpActiveLog = pActiveLog;
}

void EventLog::SaveToFile(string filePath)
{
	ofstream file;
	file.open(filePath);

	for (int i = 0; i < this->mLogEvents.size(); i++)
	{
		file << this->mEvents[i].getString() << "\n";
	}

	file.close();
}

void EventLog::LoadFromFile(string filePath)
{
}
