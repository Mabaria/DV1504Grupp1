#pragma once

#include "../Eventlog/EventLog.h"

/**
*	The room log contains pointers to all events and actions that are
*	currently active in the room. It is the link between the room and
*	the event log.
*/

class RoomLog
{
public:

	RoomLog();
	~RoomLog();
	
	// Event specific
	bool AddEvent(Event::Type type);
	bool ClearEvent(Event::Type type);

	int GetEventCount() const;
	std::vector<LogEvent*> GetActiveEvents();

	// Action specific
	bool AddAction(LogAction::Desc desc);

	bool ClearAction(int actionIndex);

	int GetActionCount() const;
	std::vector<LogAction*> GetActiveActions();

	// Room specific
	void SetRoomName(std::string name);

	// Log specific
	void SetEventLogPtr(EventLog *pEventLog);

private:

	std::vector<LogEvent*> mpEvents;
	std::vector<LogAction*> mpActions;
	EventLog *mpEventLog;
	std::string mRoomName;
};
