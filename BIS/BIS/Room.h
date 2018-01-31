#pragma once

#include <string>
#include "EventLog.h"
#include "Event.h"
#include "Sensor.h"

class Room
{
public:

	Room();
	~Room();

	// Room specific
	void SetIndex(int index);
	void SetName(std::string name);
	std::string GetName() const;

	// Deck specific
	void SetDeckName(std::string name);

	// Log specific
	void SetActiveEventIndex(int index);
	void SetEventLog(EventLog *pEventLog);

	// Event specific
	void AddEvent(Event::Type type);
	void AddInputType(Event::Type type);
	int GetActiveEventIndex() const;

	// Disk specific
	std::string WriteString() const;

private:

	// Room specific
	int mIndex;
	std::string mName;

	// Deck specific
	std::string mDeckName;

	// Log specific
	int mActiveEventIndex;

	// Sensor specific
	Sensor mSensor;
};
