#include "ActiveEvent.h"

ActiveEvent::ActiveEvent()
{
	this->mRoomIndex = -1;
}

ActiveEvent::~ActiveEvent()
{
}



/**
*	Event specific
*/

bool ActiveEvent::AddEvent(int index, LogEvent* pEvent)
{
	for (int i = 0; i < (int)this->mEvents.size(); i++)
	{
		if (this->mEvents[i].index == index)
			return false; // Event already exists
	}

	EventInfo newEvent;

	newEvent.index = index;
	newEvent.pointer = pEvent;

	this->mEvents.push_back(newEvent);
	return true;
}

bool ActiveEvent::ClearEvent(int index)
{
	for (int i = 0; i < (int)this->mEvents.size(); i++)
	{
		if (this->mEvents[i].index == index)
		{
			this->mEvents.erase(this->mEvents.begin() + i);
			return true;
		}
	}
	return false;
}

bool ActiveEvent::EventTypeExists(Event::Type type) const
{
	for (int i = 0; i < (int)this->mEvents.size(); i++)
	{
		if (this->mEvents[i].pointer->GetType() == type)
			return true;
	}

	return false;
}

int ActiveEvent::GetEventCount() const
{
	return (int)this->mEvents.size();
}

int ActiveEvent::GetEventIndexAt(int index) const
{
	return this->mEvents[index].index;
}

std::vector<LogEvent*> ActiveEvent::GetActiveEvents() const
{
	std::vector<LogEvent*> activeEvents;

	for (int i = 0; i < (int)this->mEvents.size(); i++)
	{
		activeEvents.push_back(this->mEvents[i].pointer);
	}

	return activeEvents;
}

void ActiveEvent::SetIndexInEventLog(int index)
{
	this->mIndexInEventLog = index;
}

int ActiveEvent::GetIndexInEventLog() const
{
	return this->mIndexInEventLog;
}



/**
*	Room specific
*/

void ActiveEvent::SetRoomIndex(int index)
{
	this->mRoomIndex = index;
}

int ActiveEvent::GetRoomIndex() const
{
	return this->mRoomIndex;
}
