#include "Room.h"

Room::Room()
{
	// TODO: delete these three lines, only for testing
	this->mSensor.AddInputType(Event::Fire);
	this->mSensor.AddInputType(Event::Water);
	this->mSensor.AddInputType(Event::Gas);

	this->mIndex = -1;
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

std::string Room::GetName() const
{
	return this->mName;
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
	this->mActiveEventIndex = index;
	this->mSensor.SetActiveEventIndex(index);
}

void Room::SetEventLog(EventLog *pEventLog)
{
	this->mSensor.SetEventLog(pEventLog);
}



/**
*	Event specific
*/

void Room::AddEvent(Event::Type type)
{
	this->mSensor.Trigger(type);
}

void Room::AddInputType(Event::Type type)
{
	this->mSensor.AddInputType(type);
}

int Room::GetActiveEventIndex() const
{
	return this->mActiveEventIndex;
}



/**
*	Disk specific
*/

std::string Room::WriteString() const
{
	std::string print = "";
	print += "r#" + std::to_string(this->mIndex) + " ";
	print += this->mDeckName;
	print += " / ";
	print += this->mName;
	print += " / ";
	print += this->mSensor.WriteString();

	return print;
}