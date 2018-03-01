#pragma once

#include <vector>
#include <fstream>

#include "LogEvent.h"
#include "LogAction.h"

/**
*	The log will be able to store/load content to/from a file
*	In the file, events are listed in rows. One row = one event
*
*	Example:
* 23-01-2018 14:29:21	Fire	|	started		|		Room4
* 23-01-2018 14:31:32	Water	|	stopped		|		Room1
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

	struct Desc_Action
	{
		LogAction::Type type;
		std::string roomName;
		int actionIndex;
		float pos_x;
		float pos_y;
	};

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
	LogAction* AddAction(LogAction::Desc desc);
	int GetActionCount() const;
	
	// Disk specific
	void SaveToFile(std::string filePath) const;
	bool LoadFromFile(std::string filePath);

private:
	std::vector<LogEvent*> mpLogEvents;	// Containing all events in the system
	std::vector<LogAction*> mpActions;
};
