#pragma once

#include "Event.h"

/* An event in the log will be able to describe:
	*	Type (Fire, Water, Gas)
	*	Timestamp when created, and elapsed time
	*	Which room the event is occuring in
*/

class LogEvent
{
public:
	
	LogEvent();
	LogEvent(Event::Type type, int roomEventIndex = -1);
	~LogEvent();

	// Event specific
	void SetType(Event::Type type);
	Event::Type GetType() const;

	// Log specific
	void SetActiveEventIndex(int index);
	int GetActiveEventIndex() const;

	// Time
	//const TimeObject getStartTime() const;
	//const TimeObject getElapsedTime() const;

private:

	//TimeType mTime;
	int mActiveEventIndex;
	Event::Type mType;
};
