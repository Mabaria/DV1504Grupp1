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

ActiveEvent* EventLog::AddEvent(Event::Type type, int roomIndex)
{
	int logIndex;
	int activeIndex;

	// Check if the room already has an active event.
	// In that case, update it's activeEvent
	activeIndex = GetRoomActiveEventIndex(roomIndex);
	if (activeIndex == -1)
	{
		ActiveEvent *newActiveEvent = new ActiveEvent;
		activeIndex = (int)this->mpActiveEvents.size();

		newActiveEvent->SetRoomIndex(roomIndex);
		newActiveEvent->SetIndexInEventLog(activeIndex);

		this->mpActiveEvents.push_back(newActiveEvent);
	}

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

	int size = this->mpActiveEvents[activeIndex]->GetEventCount();
	int index;

	for (int i = 0; i < size; i++)
	{
		index = this->mpActiveEvents[activeIndex]->GetEventIndexAt(i);
		if (this->mpLogEvents[index]->GetType() == type) // Found
		{
			if (!this->mpActiveEvents[activeIndex]->ClearEvent(index))
				return false; // Could not clear event

			// Check if room is not active anymore.
			// In that case, delete the active event
			if (this->mpActiveEvents[activeIndex]->GetEventCount() == 0)
			{
				this->mpActiveEvents.erase(this->mpActiveEvents.begin() + activeIndex);

				for (int i = activeIndex; i < (int)this->mpActiveEvents.size(); i++)
					this->mpActiveEvents[i]->SetIndexInEventLog(i);
			}

			return true;
		}
	}

	// The type of event wasn't active
	return false;
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
