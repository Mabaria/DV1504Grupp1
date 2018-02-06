#pragma once

#include <vector>

class ActiveEvent
{
public:

	ActiveEvent();
	~ActiveEvent();
	
	// Event specific
	bool AddEvent(int index);
	bool ClearEvent(int index);
	int GetEventCount() const;
	int operator[](int index) const;

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
