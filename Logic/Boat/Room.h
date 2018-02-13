#pragma once

#include <string>
#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"
#include "Sensor.h"

struct RoomDesc
{
	int index;
	int activeIndex;
	std::string name;
	std::string deckName;
	std::vector<Event::Type> inputTypes;
	EventLog *pEventLog;
};


class Room
{
public:

	Room();
	Room(RoomDesc desc);
	~Room();

	// Room specific
	void SetIndex(int index);
	void SetName(std::string name);
	std::string GetName() const;
	void InitFromDesc(RoomDesc desc);

	//float CheckRayCollision(Ray ray);

	// Deck specific
	void SetDeckName(std::string name);

	// Log specific
	void SetActiveEventIndex(int index);
	void SetEventLog(EventLog *pEventLog);

	// Event specific
	void AddAutoEvent(Event::Type type);
	void AddPlotEvent(Event::Type type);
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
	//int mActiveEventIndex;

	// Sensor specific
	Sensor mSensor;
};
