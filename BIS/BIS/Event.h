#pragma once

#include "EventType.h"

class Event
{
public:
	
	Event();
	Event(EventType type, int roomEventIndex = -1);
	~Event();

	// Time
	//const TimeObject getStartTime() const;
	//const TimeObject getElapsedTime() const;

	void SetRoomIndex(int index);
	void SetType(EventType type);

	int GetRoomIndex() const;
	EventType GetType() const;

private:

	//TimeType mTime;
	int mRoomEventIndex;
	EventType mType;
};
