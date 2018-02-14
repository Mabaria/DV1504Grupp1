#include "Room.h"

Room::Room()
{
	this->mIndexInBoat = -1;
	this->mIndexInDeck = -1;
	this->mDeckIndex = -1;
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
	this->mIndexInBoat = index;
	this->mSensor.SetRoomIndex(this->mIndexInBoat);
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
	this->mIndexInBoat = desc.indexInBoat;
	this->mIndexInDeck = desc.indexInDeck;
	this->mDeckIndex = desc.deckIndex;
	this->mDeckName = desc.deckName;
	this->mSensor.SetRoomIndex(desc.indexInBoat);
	this->mSensor.SetEventLog(desc.pEventLog);
	this->mSensor.SetInputTypes(desc.inputTypes);
	this->mSensor.SetActiveEventIndex(desc.activeIndex);
}

int Room::GetIndexInBoat() const
{
	return this->mIndexInBoat;
}

int Room::GetIndexInDeck() const
{
	return this->mIndexInDeck;
}




/**
*	Deck specific
*/

void Room::SetDeckName(std::string name)
{
	this->mDeckName = name;
}

std::string Room::GetDeckName() const
{
	return this->mDeckName;
}

int Room::GetDeckIndex() const
{
	return this->mDeckIndex;
}



/**
*	Log specific
*/

void Room::SetActiveEventIndex(int index)
{
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
