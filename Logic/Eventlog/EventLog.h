#pragma once

#include <vector>
#include <fstream>

#include "LogEvent.h"
#include "ActiveEvent.h"

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
	ActiveEvent* AddEvent(Event::Type type, int roomIndex);	/* Pushing an event
																										at the end of the list,
																										returns index of event in
																										the active log */
	bool ClearEvent(Event::Type type, int roomIndex);
	std::vector<Event::Type> GetEvents(int roomIndex) const;
	std::vector<Event::Type> GetEvents(std::string roomName) const;
	int GetEventCount() const;
	int GetActiveEventCount() const;

	LogEvent* GetEventPointer(int index);
	ActiveEvent* GetActiveEventPointer(int index);
	
	// Disk specific
	void SaveToFile(std::string filePath);
	void LoadFromFile(std::string filePath);

private:

	int GetRoomActiveEventIndex(int roomIndex) const;

	std::vector<LogEvent*> mpLogEvents;	// Containing all events in the system
	std::vector<ActiveEvent*> mpActiveEvents;
};
