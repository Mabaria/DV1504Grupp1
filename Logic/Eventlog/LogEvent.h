#pragma once

#include <sstream>
#include "../Event/Event.h"
#include "../Timer.h"
#include "../CorrectName.h"

/* An event in the log will be able to describe:
	*	Type (Fire, Water, Gas)
	*	Timestamp when created, and elapsed time
	*	Which room the event is occuring in
*/

class LogEvent
{
public:

	struct Desc
	{
		Event::Type type;
		std::string roomName;
		bool start; // Whether the event is a start or a clear
		bool active;

		int ID; // Will be filled automaticaly
	};

	LogEvent();
	LogEvent(LogEvent::Desc desc);
	LogEvent(std::string lineFromLog, int ID);
	~LogEvent();

	// Event specific
	void SetType(Event::Type type);
	Event::Type GetType() const;
	bool IsActive() const;
	bool IsStartEvent() const;
	void SetInactive();
	void SetActive();
	int GetID() const;

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
	Timestamp GetTimestamp_Start() const;
	
	Timer *GetTimer();


private:

	Timer mTimer;

	bool mActive;
	bool mStartEvent;
	Event::Type mType;
	std::string mRoomName;

	// Used when saving/reading from file
	int mID;
};
