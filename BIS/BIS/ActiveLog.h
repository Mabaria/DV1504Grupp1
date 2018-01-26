#pragma once

#include <vector>
#include "RoomEvent.h"

class ActiveLog 
{
public:

	ActiveLog();
	~ActiveLog();

	// Will return index of RoomEvent
	int AddEvent(int eventIndex, int roomIndex);

	void RemoveEvent(int index);

	//RoomEvent operator[](int index);

private:
	
	// Active events
	std::vector<RoomEvent> mRoomEvents;
};
