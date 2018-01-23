#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
}

bool EventLog::AddEvent(EventType type, EventState state, Room *room)
{
	this->mEvents.push_back(Event(type, state, room));
}

// Get event as a string to print
// [DD.MM.YYYY] [hh:mm] <<Event type>>, Started/Stopped
const std::string EventLog::GetEventStringAt(int index) const
{
	return this->mEvents[i].getString();
}

void EventLog::SaveToFile(string filePath)
{
	ofstream file;
	file.open(filePath);

	for (int i = 0; i < this->mEvents.size(); i++)
	{
		file << this->mEvents[i].getString() << "\n";
	}

	file.close();
}

void EventLog::LoadFromFile(string filePath)
{
}
