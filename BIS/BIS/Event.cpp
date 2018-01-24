#include "Event.h"

Event::Event()
{

}

Event::Event(EventType type, int roomEventIndex)
{
	this->mType = type;
	this->mRoomEventIndex = roomEventIndex;
}

Event::~Event()
{

}

//const TimeObject Event::getStartTime() const;
//const TimeObject Event::getElapsedTime() const;

void Event::SetRoomIndex(int index)
{
	this->mRoomEventIndex = index;
}

void Event::SetType(EventType type)
{
	this->mType = type;
}

int Event::GetRoomIndex() const
{
	return this->mRoomEventIndex;
}

EventType Event::GetType() const
{
	return this->mType;
}
