#pragma once

#include <vector>
#include "RoomEvent.h"

class ActiveLog 
{
public:

	ActiveLog();
	~ActiveLog();

	void AddEvent(int eventIndex, int roomIndex);
	void RemoveEvent(int index);

	RoomEvent operator[](int index);

private:
	
	// Indices to active (room) events
	std::vector<RoomEvent> mEventIndices;
};
