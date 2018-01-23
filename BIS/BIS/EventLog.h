#pragma once

#include <vector>
#include <fstream>

#include "Event.h"

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

	bool AddEvent(EventType type, EventState state, Room room);
	
	// Get event as a string to print
	// [DD.MM.YYYY] [hh:mm] <<Event type>>, Started/Stopped
	const std::string GetEventStringAt(int index) const;

	void SaveToFile(string filePath);
	void LoadFromFile(string filePath);

private:

	std::vector<Event> mEvents;
		
}
