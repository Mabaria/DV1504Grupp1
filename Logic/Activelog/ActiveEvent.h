#pragma once

#include <vector>

class ActiveEvent
{
public:

	ActiveEvent();
	~ActiveEvent();
	
	// Event specific
	void AddEvent(int index);
	void RemoveEvent(int index);

	// Room specific
	void SetRoomIndex(int index);
	int GetRoomIndex() const;

private:
	
	// Event specific
	std::vector<int> mEventIndices; /* Points to all events in the eventLog
																		 that are active in the room */
	// Room specific
	int mRoomIndex;
};
