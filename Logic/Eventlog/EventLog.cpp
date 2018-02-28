#include "EventLog.h"

EventLog::EventLog()
{
}

EventLog::~EventLog()
{
	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		delete this->mpLogEvents[i];
	}

	for (int i = 0; i < (int)this->mpActions.size(); i++)
	{
		delete this->mpActions[i];
	}
}



/**
*	Event specific
*/

LogEvent* EventLog::AddEvent(Event::Type type, std::string roomName)
{
	this->mpLogEvents.push_back(new LogEvent(type, roomName));
	return this->mpLogEvents.back();
}

int EventLog::GetEventCount() const
{
	return this->mpLogEvents.size();
}

LogEvent* EventLog::GetEventPointer(int index)
{
	if (index < 0 || index >= (int)this->mpLogEvents.size())
		return nullptr;
	
	return this->mpLogEvents[index];
}

/**
*	Action specific
*/

Action* EventLog::AddAction(ActionType type, std::string roomName)
{
	// TODO
	this->mpActions.push_back(new Action(type, roomName));
	return this->mpActions.back();
}

int EventLog::GetActionCount() const
{
	return (int)this->mpActions.size();
}



/**
*	Disk specific
*/

void EventLog::SaveToFile(std::string filePath) const
{
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		file << "e  " << this->mpLogEvents[i]->GetFileString() << std::endl;
	}

	file.close();
}

bool EventLog::LoadFromFile(std::string filePath)
{
	std::ifstream file(filePath);

	std::string line;
	std::string word;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			// Check first char in line
			switch (line[0])
			{
				case 'e':	// Event
					this->mpLogEvents.push_back(new LogEvent(line));
					break;
			}
		}
	}
	else
		return false;
	
	return true;
}
