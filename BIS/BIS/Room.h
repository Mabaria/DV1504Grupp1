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

	void AddEvent(Event::Type type);
	void AddInputType(Event::Type type);

	void SetIndex(int index);
	void SetName(std::string name);
	void SetDeckName(std::string name);
	void SetRoomEventIndex(int index);
	
	void SetEventLog(EventLog *pEventLog);

	std::string GetName() const;
	int GetRoomEventIndex() const;

	std::string WriteString() const;

private:

	// Name and index of this room
	int mIndex;
	std::string mName;
	std::string mDeckName;

	int mRoomEventIndex;
	Sensor mSensor;
};
