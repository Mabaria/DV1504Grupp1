#pragma once

#include <vector>
#include <fstream>

#include "LogEvent.h"
#include "../Activelog/ActiveLog.h"


/**
*	The log will be able to store/load content to/from a file
*	In the file, events are listed in rows. One row = one event
*
*	Example:
* 23.01.2018 14:29 Fire	started in Room4, Deck2
* 23.01.2018 14:31 Fire stopped in Room4, Deck2
*	
*/

/**
* BEFORE USING EVENTLOG:
*
*	EventLog need an ActiveLog to report active events to!
*/

// TODO: Set filepath

class EventLog
{
public:

	EventLog();
	~EventLog();

	// Log specific
	void SetActiveLog(ActiveLog *pActiveLog);

	// Event Specific
	int AddLogEvent(Event::Type type, int roomIndex); /* Pushing an event at the
																											 end of the list, returns
																											 index of event in the
																											 active log */
	
	// Disk specific
	void SaveToFile(std::string filePath);
	void LoadFromFile(std::string filePath);

private:

	std::vector<LogEvent> mLogEvents;	// Containing all events in the system
	ActiveLog *mpActiveLog;
};
