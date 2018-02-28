#pragma once

#include <vector>
#include <fstream>

#include "LogEvent.h"
#include "ActiveEvent.h"
#include "../Action/Action.h"

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

// TODO: Read/Write events

class EventLog
{
public:

	EventLog();
	~EventLog();

	// Event Specific
	LogEvent* AddEvent(Event::Type type, std::string roomName);
															/* Pushing an event
															at the end of the list,
															returns index of event in
															the active log */
	int GetEventCount() const;
	LogEvent* GetEventPointer(int index);

	// Action specific
	Action* AddAction(ActionType type, std::string roomName);
	int GetActionCount() const;
	
	// Disk specific
	void SaveToFile(std::string filePath) const;
	bool LoadFromFile(std::string filePath);

private:
	std::vector<LogEvent*> mpLogEvents;	// Containing all events in the system
	std::vector<Action*> mpActions;
};
