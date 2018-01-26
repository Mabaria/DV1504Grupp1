#pragma once

#include "Event.h"

class LogEvent
{
public:
	
	LogEvent();
	LogEvent(Event::Type type, int roomEventIndex = -1);
	~LogEvent();

	// Time
	//const TimeObject getStartTime() const;
	//const TimeObject getElapsedTime() const;

	void SetRoomEventIndex(int index);
	void SetType(Event::Type type);

	int GetRoomEventIndex() const;
	Event::Type GetType() const;

private:

	//TimeType mTime;
	int mRoomEventIndex;
	Event::Type mType;
};
