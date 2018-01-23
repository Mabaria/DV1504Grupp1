#include "Event.h"

// Event(Time time, EventType type, EventState state, Room *room)
Event::Event(EventType type, EventState state, Room *room)
{
	this->mType = type;
	this->mState = state;
	this->mpRoom = room;
}

Event::~Event()
{

}

//const TimeObject Event::getStartTime() const;
//const TimeObject Event::getElapsedTime() const;

std::string Event::getString() const
{
	std::string line = "";

	// line += date and time
	
	switch (this->mType)
	{
		case Fire:
			line += " A fire";
			break;
		case Water:
			line += " A water leakage";
			break;
		case Gas:
			line += " A gas leakage";
			break;
		default:
			line += " UNKNOWN_EVENT_TYPE";
			break;
	}

	switch (this->mState)
	{
		case Start:
			line += " has started";
			break;
		case Stop:
			line += " has been taken care of";
			break;
		default:
			line += " UNKNOWN_EVENT_STATE";
			break;
	}

	line += this->mpRoom->GetName() + ", " + this->mpRoom->GetDeck() + ".";
}
