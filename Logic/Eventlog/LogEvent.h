#pragma once

#include <sstream>
#include "../Event/Event.h"
#include "../../Logic/Timer.h"

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

	std::string GetFileString();

	// Time specific
	void StartTimer();
	const int GetElapsedTime();
	const std::string GetStartTimeAsString();
	const std::string GetElapsedTimeAsString();
	
	Timer *GetTimer();


private:

	Timer mTimer;
	int mActiveEventIndex;
	Event::Type mType;
};
