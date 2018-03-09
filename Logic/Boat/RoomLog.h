#pragma once

#include "../Log/Log.h"

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
	void SetEventLogPtr(Log *pEventLog);

	// Disk specific
	void SetMetaPath(std::string path);
	void ClearMeta() const;

	void SaveToFile(std::string folderPath) const;
	bool LoadFromFile(std::string folderPath);

	void SaveToFile() const;
	bool LoadFromFile();

private:

	std::string RealPath(std::string folderPath) const;
	void UpdateMeta() const;

	std::vector<LogEvent*> mpEvents;
	std::vector<LogAction*> mpActions;
	Log *mpEventLog;
	std::string mRoomName;

	std::string mMetaPath;
};
