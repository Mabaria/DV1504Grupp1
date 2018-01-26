#include "Room.h"

Room::Room()
{
	this->mSensor.AddInputType(Event::Fire);
	this->mSensor.AddInputType(Event::Water);
	this->mSensor.AddInputType(Event::Gas);

	this->mIndex = -1;
}

Room::~Room()
{

}


void Room::AddEvent(Event::Type type)
{
	this->mSensor.Trigger(type);
}

void Room::SetIndex(int index)
{
	this->mSensor.SetRoomIndex(this->mIndex);
}

void Room::SetName(std::string name)
{
	this->mName = name;
}

void Room::SetDeckName(std::string name)
{
	this->mDeckName = name;
}

void Room::SetRoomEventIndex(int index)
{
	this->mRoomEventIndex = index;
	this->mSensor.SetRoomEventIndex(index);
}

void Room::SetEventLog(EventLog *pEventLog)
{
	this->mSensor.SetEventLog(pEventLog);
}

std::string Room::GetName() const
{
	return this->mName;
}

int Room::GetRoomEventIndex() const
{
	return this->mRoomEventIndex;
}

std::string Room::WriteString() const
{
	std::string print = "";
	print += "r#" + this->mIndex;
	print += this->mDeckName;
	print += " / ";
	print += this->mName;
	print += " / ";
	print += this->mSensor.WriteString();

	return print;
}
