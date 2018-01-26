#include "EventLog.h"

EventLog::EventLog()
{
	//int nr = this->mLogEvents.size();
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
