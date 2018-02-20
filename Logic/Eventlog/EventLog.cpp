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

	for (int i = 0; i < (int)this->mpActiveEvents.size(); i++)
	{
		delete this->mpActiveEvents[i];
	}
}



/**
*	Event specific
*/

ActiveEvent* EventLog::AddEvent(Event::Type type, int roomIndex, Observer<ActiveEvent> *pObserver)
{
	int logIndex;
	int activeIndex;

	bool newActive = false;

	// Check if the room already has an active event.
	// In that case, update it's activeEvent
	activeIndex = GetRoomActiveEventIndex(roomIndex);
	if (activeIndex == -1)
	{
		newActive = true;
		ActiveEvent *newActiveEvent = new ActiveEvent;
		newActiveEvent->AddObserver(pObserver);
		activeIndex = (int)this->mpActiveEvents.size();

		newActiveEvent->SetRoomIndex(roomIndex);
		newActiveEvent->SetIndexInEventLog(activeIndex);

		this->mpActiveEvents.push_back(newActiveEvent);
	}

	// Check if event type already is active in room
	if (!this->mpActiveEvents[activeIndex]->EventTypeExists(type) || newActive)
	{
		// Create new log event
		LogEvent *newLogEvent = new LogEvent;

		newLogEvent->SetActiveEventIndex(activeIndex);
		newLogEvent->SetType(type);

		logIndex = (int)this->mpLogEvents.size();

		this->mpLogEvents.push_back(newLogEvent);
		this->mpActiveEvents[activeIndex]->AddEvent(
			logIndex,
			this->mpLogEvents.back());

		return this->mpActiveEvents[activeIndex];
	}
	
	// Event type already exists in room
	return nullptr;
}

bool EventLog::ClearEvent(Event::Type type, int roomIndex)
{
	int activeIndex = -1;

	// Check if room is active
	for (int i = 0; i < (int)this->mpActiveEvents.size(); i++)
	{
		if (this->mpActiveEvents[i]->GetRoomIndex() == roomIndex)
			activeIndex = i;
	}

	if (activeIndex == -1)
		return false; // Nothing to clear

	return this->mpActiveEvents[activeIndex]->ClearEvent(type);
}

void EventLog::ClearActiveEvent(int index)
{
	if (index < 0 || index > (int)this->mpActiveEvents.size())
		return;

	delete this->mpActiveEvents[index];
	this->mpActiveEvents.erase(this->mpActiveEvents.begin() + index);

	for (int i = index; i < (int)this->mpActiveEvents.size(); i++)
	{
		this->mpActiveEvents[i]->SetIndexInEventLog(i);
		this->mpActiveEvents[i]->NotifyObservers(this->mpActiveEvents[i]);
	}
}

std::vector<Event::Type> EventLog::GetEvents(int roomIndex) const
{
	std::vector<Event::Type> returnEvents;

	int size;
	int activeIndex = this->GetRoomActiveEventIndex(roomIndex);
	
	if (activeIndex != -1)
		size = this->mpActiveEvents[activeIndex]->GetEventCount();
	else
		size = 0;
	
	returnEvents.reserve(size);
	int index;

	for (int i = 0; i < size; i++)
	{
		index = this->mpActiveEvents[activeIndex]->GetEventIndexAt(i);
		returnEvents.push_back(this->mpLogEvents[index]->GetType());
	}

	return returnEvents;
}

int EventLog::GetEventCount() const
{
	return this->mpLogEvents.size();
}

int EventLog::GetActiveEventCount() const
{
	return this->mpActiveEvents.size();
}

LogEvent* EventLog::GetEventPointer(int index)
{
	if (index < 0 || index >= (int)this->mpLogEvents.size())
		return nullptr;
	
	return this->mpLogEvents[index];
}

ActiveEvent* EventLog::GetActiveEventPointer(int index)
{
	if (index < 0 || index >= (int)this->mpActiveEvents.size())
		return nullptr;
	
	return this->mpActiveEvents[index];
}


/**
*	Disk specific
*/

void EventLog::SaveToFile(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < (int)this->mpLogEvents.size(); i++)
	{
		file << "EVENT TEXT" << "\n";
	}

	file.close();
}

void EventLog::LoadFromFile(std::string filePath)
{
}


/**
*	Private
*/

int EventLog::GetRoomActiveEventIndex(int roomIndex) const
{
	for (int i = 0; i < (int)this->mpActiveEvents.size(); i++)
	{
		if (this->mpActiveEvents[i]->GetRoomIndex() == roomIndex)
			return i;
	}

	// Return -1 if room doesn't have any active events
	return -1;
}
