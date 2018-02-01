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

//void EventLog::SetActiveLog(ActiveLog *pActiveLog)
//{
//	this->mpActiveLog = pActiveLog;
//}



/**
*	Event specific
*/

int EventLog::AddEvent(Event::Type type, int roomIndex)
{
	int logIndex;
	int activeIndex;

	// Check if the room already has an active event.
	// In that case, update it's activeEvent
	activeIndex = GetRoomActiveEventIndex(roomIndex);
	if (activeIndex == -1)
	{
		ActiveEvent newActiveEvent;

		newActiveEvent.SetRoomIndex(roomIndex);

		activeIndex = (int)this->mActiveEvents.size();
		this->mActiveEvents.insert(
			this->mActiveEvents.begin() + activeIndex,
			newActiveEvent);
	}

	LogEvent newLogEvent;

	newLogEvent.SetActiveEventIndex(activeIndex);
	newLogEvent.SetType(type);

	logIndex = (int)this->mLogEvents.size();

	this->mLogEvents.insert(this->mLogEvents.begin() + logIndex, newLogEvent);
	this->mActiveEvents[activeIndex].AddEvent(logIndex);

	return activeIndex;
}

bool EventLog::ClearEvent(Event::Type type, int roomIndex)
{
	int activeIndex = -1;

	// Check if room is active
	for (int i = 0; i < (int)this->mActiveEvents.size(); i++)
	{
		if (this->mActiveEvents[i].GetRoomIndex() == roomIndex)
			activeIndex = i;
	}

	if (activeIndex == -1)
		return false; // Nothing to clear

	int size = this->mActiveEvents[activeIndex].GetEventCount();
	int index;

	for (int i = 0; i < size; i++)
	{
		index = (this->mActiveEvents[activeIndex])[i];
		if (this->mLogEvents[index].GetType() == type) // Found
		{
			if (!this->mActiveEvents[activeIndex].ClearEvent(index))
				return false; // Could not clear event

			// Check if room is not active anymore.
			// In that case, delete the active event
			if (this->mActiveEvents[activeIndex].GetEventCount() == 0)
			{
				this->mActiveEvents.erase(this->mActiveEvents.begin() + activeIndex);
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
		size = this->mActiveEvents[activeIndex].GetEventCount();
	else
		size = 0;
	
	returnEvents.reserve(size);
	int index;

	for (int i = 0; i < size; i++)
	{
		index = (this->mActiveEvents[activeIndex])[i];
		returnEvents.push_back(this->mLogEvents[index].GetType());
	}

	return returnEvents;
}

int EventLog::GetEventCount() const
{
	return this->mLogEvents.size();
}

int EventLog::GetActiveEventCount() const
{
	return this->mActiveEvents.size();
}



/**
*	Disk specific
*/

void EventLog::SaveToFile(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);

	for (int i = 0; i < (int)this->mLogEvents.size(); i++)
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
	for (int i = 0; i < (int)this->mActiveEvents.size(); i++)
	{
		if (this->mActiveEvents[i].GetRoomIndex() == roomIndex)
			return i;
	}

	// Return -1 if room doesn't have any active events
	return -1;
}
