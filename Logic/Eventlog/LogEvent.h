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
	LogEvent(Event::Type type, std::string roomName);
	LogEvent(std::string lineFromLog);
	~LogEvent();

	// Event specific
	void SetType(Event::Type type);
	Event::Type GetType() const;
	bool IsActive() const;
	void SetInactive();

	// Log specific
	std::string GetFileString();

	// Room specific
	void SetRoomName(std::string name);
	std::string GetRoomName() const;

	// Time specific
	void StartTimer();
	const int GetElapsedTime();
	const std::string GetStartTimeAsString();
	const std::string GetElapsedTimeAsString();
	
	Timer *GetTimer();


private:

	Timer mTimer;

	bool mActive;
	Event::Type mType;
	std::string mRoomName;
};
