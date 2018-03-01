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
 
bool RoomLog::AddAction(LogAction::Type type, float pos_x, float pos_z)
{
	LogAction *pNewAction = this->mpEventLog->AddAction(
		type,
		this->mRoomName,
		pos_x,
		pos_z);

	this->mpActions.push_back(pNewAction);
	return true;
}

// FIXME when Graphical pointers exists

//bool RoomLog::ClearAction(GraphicalAction *pAction)
//{
//	for (int i = 0; i < (int)this->mpActions.size(); i++)
//	{
//		if (this->mpActions[i]->GetGraphicalActionPointer() == pAction)
//		{
//			this->mpActions[i]->SetInactive();
//			return true;
//		}
//	}
//
//	return false;
//}

int RoomLog::GetActionCount() const
{
	return (int)this->mpActions.size();
}

std::vector<LogAction*> RoomLog::GetActiveActions()
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
