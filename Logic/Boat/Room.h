#pragma once

#include <string>
#include "../../IO/Picking.h"
#include "../Eventlog/EventLog.h"
#include "RoomLog.h"
#include "../Event/Event.h"
#include "Sensor.h"
#include "../Eventlog/LogAction.h"

class Room
{
public:

	struct Desc
	{
		int inputs;
		int index_Boat;
		int index_Deck;
		int index_DeckInBoat;
		std::string name;
		std::string deckName;
		EventLog *pEventLog;
	};



	Room();
	Room(Room::Desc desc);
	~Room();

	// Room specific
	void SetIndex_Boat(int index);
	void SetIndex_Deck(int index);
	void SetName(std::string name);
	void SetAABB(const AABB &boundingBox);
	std::string GetName() const;
	void InitFromDesc(Room::Desc desc);
	int GetIndexInBoat() const;
	int GetIndexInDeck() const;

	float CheckRayCollision(const Ray &rRay);

	// Deck specific
	void SetDeckName(std::string name);
	std::string GetDeckName() const;
	int GetIndex_Boat() const;
	int GetIndex_Deck() const;
	int GetIndex_DeckInBoat() const;

	// Event specific
	bool AddSensorEvent(Event::Type type);	/* This function will only add
											   event if sensor is capable
											   of detecting the event type*/

	bool AddPlotterEvent(Event::Type type);	/* This function will add the
											   event without checking the
											   sensor */
	bool ClearEvent(Event::Type type);

	void AddInputType(Event::Type type);
	std::vector<LogEvent*> GetActiveEvents();

	// Action specific
	//bool AddAction(LogAction::Type type, GraphicalAction *pAction); // FIXME
	//bool ClearAction(GraphicalAction *pAction); // FIXME when Graphical actions exists

	// Disk specific
	std::string GetString() const;
	static Room::Desc FillRoomDescFromLine(std::string line);
	static std::string CorrectName(std::string name);

private:

	// Room specific
	int mIndex_Boat;
	int mIndex_Deck;
	int mIndex_DeckInBoat;
	std::string mName;
	AABB mBoundingBox;

	// Deck specific
	std::string mDeckName;

	// Sensor specific
	Sensor mSensor;

	// Log specific
	RoomLog mRoomLog;
};
