#pragma once

#include <string>
#include "../../IO/Picking.h"
#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"
#include "Sensor.h"

//#include "Boat.h"

struct RoomDesc
{
	int indexInBoat;
	int indexInDeck;
	int deckIndex;
	int activeIndex;
	ActiveEvent *pActiveEvent;
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
	void SetAABB(const AABB &boundingBox);
	std::string GetName() const;
	void InitFromDesc(RoomDesc desc);
	int GetIndexInBoat() const;
	int GetIndexInDeck() const;

	float CheckRayCollision(const Ray &rRay);

	// Deck specific
	void SetDeckName(std::string name);
	std::string GetDeckName() const;
	int GetDeckIndex() const;

	// Log specific
	void SetActiveEvent(int index, ActiveEvent *pActiveEvent);
	void SetEventLog(EventLog *pEventLog);

	// Event specific
	bool AddSensorEvent(Event::Type type);	/* This function will only add
											   event if sensor is capable
											   of detecting the event type*/

	bool AddPlotterEvent(Event::Type type);	/* This function will add the
											   event without checking the
											   sensor */
	bool ClearEvent(Event::Type type);

	void AddInputType(Event::Type type);
	int GetActiveEventIndex() const;
	std::vector<LogEvent*> GetActiveEvents() const;

	// Disk specific
	std::string WriteString() const;

private:

	// Room specific
	int mIndexInBoat;
	int mIndexInDeck;
	int mDeckIndex;
	std::string mName;
	AABB mBoundingBox;

	// Deck specific
	std::string mDeckName;

	// Sensor specific
	Sensor mSensor;
};
