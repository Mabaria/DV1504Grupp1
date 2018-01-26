#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "Event.h"
#include "EventLog.h"

class Sensor
{
public:

	Sensor();
	~Sensor();

	void AddInputType(Event::Type type);
	void RemoveInputType(Event::Type type);
	void SetEventLog(EventLog *pEventLog);
	void SetRoomIndex(int index);
	void SetRoomEventIndex(int index);

	bool CanDetect(Event::Type type) const;

	// Will return a RoomEvent index
	// (intex to the active event)
	int Trigger(Event::Type type);

	std::string WriteString() const;

private:

	// A vector makes it able to adapt later on when 
	// more types of events get added to the program
	std::vector<Event::Type> mInputTypes;

	int mRoomEventIndex;
	int mRoomIndex;		// Index to this sensors room

	EventLog *mpEventLog;
};
