#pragma once

#include <vector>
#include "EventLog.h"

class RoomEvent
{
public:

	RoomEvent(int roomIndex);
	~RoomEvent();
	
	void AddEvent(int index);
	void SetRoomIndex(int index);

	int GetRoomIndex() const;
	int operator[](int index) const;

private:

	std::vector<int> mEventIndices;
	int mRoomIndex;
};
