#pragma once

#include <vector>
#include <fstream>

#include "LogEvent.h"
#include "ActiveLog.h"

/* An event should be able to describe:
	* A danger event (Fire, Water, Gas etc.)
	*	
	*
	*
	*
*/

/**
*	The log will be able to store/load content to/from a file
*
*	In the file, events are listed in rows. One row = one event
*
*	Example:
* 23.01.2018 14:29 Fire	started in Room4, Deck2
* 23.01.2018 14:31 Fire stopped in Room4, Deck2
*	
*
*/

class EventLog
{
public:

	EventLog();
	~EventLog();

	int AddLogEvent(Event::Type type, int roomIndex);

	void SetActiveLog(ActiveLog *pActiveLog);
	
	void SaveToFile(std::string filePath);
	void LoadFromFile(std::string filePath);

private:

	std::vector<LogEvent> mLogEvents;
	ActiveLog *mpActiveLog;
};
