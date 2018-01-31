#pragma once

#include <vector>
#include "ActiveEvent.h"

class ActiveLog 
{
public:

	ActiveLog();
	~ActiveLog();

	// Event specific
	int AddEvent(int eventIndex, int roomIndex); /* Returns the index of the
																									added event in the active
																									list */
	void RemoveEvent(int index);

private:
	
	// Event specific
	std::vector<ActiveEvent> mActiveEvents;
};
