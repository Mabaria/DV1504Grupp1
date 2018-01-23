#pragma once

#include <vector>
#include <sstream>

#include "Room.h"

// All types of an event.
// Row-style allows an easy management of adding/ removing types to list
enum EventType
{
	Fire,
	Water,
	Gas
};

enum EventState
{
	Start,
	Stop
};

class Event
{
public:
	
	// Event(Time time, EventType type, EventState state, Room *room);
	Event(EventType type, EventState state, Room *room);
	~Event();

	// Time
	//const TimeObject getStartTime() const;
	//const TimeObject getElapsedTime() const;

	std::string getString() const;


private:

	//TimeType mStartTime;
	EventType mType;
	EventState mState;

	Room* mpRoom;
};
