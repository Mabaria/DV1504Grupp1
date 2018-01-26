#pragma once

#include <vector>

class RoomEvent
{
public:

	RoomEvent();
	~RoomEvent();
	
	void AddEvent(int index);
	void SetRoomIndex(int index);

	int GetRoomIndex() const;
	//int operator[](int index) const;

private:

	std::vector<int> mEventIndices;
	int mRoomIndex;
};
