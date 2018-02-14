#include "Room.h"

Room::Room()
{
	this->mIndex = -1;
}

Room::Room(RoomDesc desc)
{
	this->InitFromDesc(desc);
}

Room::~Room()
{

}



/**
*	Room specific
*/

void Room::SetIndex(int index)
{
	this->mIndex = index;
	this->mSensor.SetRoomIndex(this->mIndex);
}

void Room::SetName(std::string name)
{
	this->mName = name;
}

void Room::SetAABB(const AABB &boundingBox)
{
	this->mBoundingBox = boundingBox;
}

float Room::CheckRayCollision(const Ray & rRay)
{
	return Picking::IsRayIntersectingAABB(rRay, this->mBoundingBox);
}

std::string Room::GetName() const
{
	return this->mName;
}

void Room::InitFromDesc(RoomDesc desc)
{
	this->mName = desc.name;
	this->mIndex = desc.index;
	this->mDeckName = desc.deckName;
	this->mSensor.SetRoomIndex(desc.index);
	this->mSensor.SetEventLog(desc.pEventLog);
	this->mSensor.SetInputTypes(desc.inputTypes);
	this->mSensor.SetActiveEventIndex(desc.activeIndex);
}



/**
*	Deck specific
*/

void Room::SetDeckName(std::string name)
{
	this->mDeckName = name;
}



/**
*	Log specific
*/

void Room::SetActiveEventIndex(int index)
{
	//this->mActiveEventIndex = index;
	this->mSensor.SetActiveEventIndex(index);
}

void Room::SetEventLog(EventLog *pEventLog)
{
	this->mSensor.SetEventLog(pEventLog);
}



/**
*	Event specific
*/

void Room::AddAutoEvent(Event::Type type)
{
	this->mSensor.AutoTrigger(type);
}

void Room::AddPlotEvent(Event::Type type)
{
	this->mSensor.PlotTrigger(type);
}

void Room::AddInputType(Event::Type type)
{
	this->mSensor.AddInputType(type);
}

int Room::GetActiveEventIndex() const
{
	//return this->mActiveEventIndex;
	return this->mSensor.GetActiveEventIndex();
}



/**
*	Disk specific
*/

std::string Room::WriteString() const
{
	std::string print = "";

	print += "r ";
	print += this->mName;
	print += " / ";
	print += this->mSensor.WriteString();

	return print;
}
