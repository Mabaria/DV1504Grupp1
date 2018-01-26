#include "RoomEvent.h"

RoomEvent::RoomEvent()
{
	this->mRoomIndex = -1;
}

RoomEvent::~RoomEvent()
{
}

void RoomEvent::AddEvent(int index)
{
	this->mEventIndices.push_back(index);
}

void RoomEvent::SetRoomIndex(int index)
{
	this->mRoomIndex = index;
}

int RoomEvent::GetRoomIndex() const
{
	return this->mRoomIndex;
}

//int RoomEvent::operator[](int index) const
//{
//	return this->mEventIndices[index];
//}
