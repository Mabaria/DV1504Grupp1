#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "../Event/Event.h"
#include "../Eventlog/EventLog.h"

class Sensor
{
public:

	Sensor();
	~Sensor();

	// Sensor specific
	void AddInputType(Event::Type type);
	void RemoveInputType(Event::Type type);
	bool CanDetect(Event::Type type) const;
	int Trigger(Event::Type type); // Will return a ActiveEvent index

	// Log specific
	void SetEventLog(EventLog *pEventLog);

	// Event specific
	void SetActiveEventIndex(int index);

	// Room specific
	void SetRoomIndex(int index);
	
	// Disk specific
	std::string WriteString() const;

private:

	// Sensor specific
	std::vector<Event::Type> mInputTypes;	/* Holds all the type of events the
																					 sensor can detect */
	// Log specific
	EventLog *mpEventLog;

	// Event specific
	int mActiveEventIndex;
	
	// Room specific
	int mRoomIndex;	// Index to the room this sensor is in
};
