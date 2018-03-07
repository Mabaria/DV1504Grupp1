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
	void GetActiveEvents(std::vector<LogEvent*> &output);

	// Action specific
	bool AddAction(LogAction::Desc desc);

	bool ClearAction(int *actionIndex);

	int GetActionCount() const;
	void GetActiveActions(std::vector<LogAction*> &output);

	// Room specific
	void SetRoomName(std::string name);

	// Log specific
	void SetEventLogPtr(EventLog *pEventLog);

	// Disk specific
	void SaveToFile(std::string folderPath) const;
	bool LoadFromFile(std::string folderPath);

private:

	std::string RealPath(std::string folderPath) const;

	std::vector<LogEvent*> mpEvents;
	std::vector<LogAction*> mpActions;
	EventLog *mpEventLog;
	std::string mRoomName;
};
