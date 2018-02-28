#include "RoomLog.h"

RoomLog::RoomLog()
{
	mpEventLog = nullptr;
}

RoomLog::~RoomLog()
{
	
}



/**
*	Event specific
*/

bool RoomLog::AddEvent(Event::Type type)
{
	for (int i = 0; i < (int)this->mpEvents.size(); i++)
	{
		// Check if type is already active in room
		if (this->mpEvents[i]->GetType() == type)
			return false;
	}

	LogEvent *pNewEvent = this->mpEventLog->AddEvent(type, this->mRoomName);
	this->mpEvents.push_back(pNewEvent);
	return true;
}

bool RoomLog::ClearEvent(Event::Type type)
{
	for (int i = 0; i < (int)this->mpEvents.size(); i++)
	{
		// Find event with requested type
		if (this->mpEvents[i]->GetType() == type)
		{
			this->mpEvents[i]->SetInactive();
			this->mpEvents.erase(this->mpEvents.begin() + i);
			return true;
		}
	}

	return false;
}

int RoomLog::GetEventCount() const
{
	return (int)this->mpEvents.size();
}

std::vector<LogEvent*> RoomLog::GetActiveEvents()
{
	return this->mpEvents;
}



/**
*	Action specific
*/
 
bool RoomLog::AddAction(ActionType type)
{
	// TODO
	return false;
}

bool RoomLog::ClearAction(ActionType type)
{
	// TODO
	return false;
}

int RoomLog::GetActionCount() const
{
	return (int)this->mpActions.size();
}

std::vector<Action*> RoomLog::GetActiveActions()
{
	return this->mpActions;
}

/**
*	Room specific
*/

void RoomLog::SetRoomName(std::string name)
{
	this->mRoomName = name;
}



/**
*	Log specific
*/

void RoomLog::SetEventLogPtr(EventLog *pEventLog)
{
	this->mpEventLog = pEventLog;
}
