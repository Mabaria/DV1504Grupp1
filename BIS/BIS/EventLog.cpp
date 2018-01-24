#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
}

void EventLog::AddEvent(EventType type, int roomEventIndex)
{
	this->mEvents.push_back(Event(type, roomEventIndex));
}

void EventLog::SaveToFile(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < this->mEvents.size(); i++)
	{
		file << "EVENT TEXT" << "\n";
	}

	file.close();
}

void EventLog::LoadFromFile(std::string filePath)
{
}
