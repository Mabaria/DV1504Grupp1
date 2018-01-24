#include "Room.h"

Room::Room()
{
}

Room::~Room()
{

}


void Room::AddEvent(EventType type)
{
	// TODO
}

void Room::SetName(std::string name)
{
	this->mName = name;
}

void Room::SetRoomEventIndex(int index)
{
	this->mRoomEventIndex = index;
}

std::string Room::GetName() const
{
	return this->mName;
}

int Room::GetRoomEventIndex() const
{
	return this->mRoomEventIndex;
}
