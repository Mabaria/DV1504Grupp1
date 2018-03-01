#pragma once

#include <string>
#include "../../IO/Picking.h"
#include "../Eventlog/EventLog.h"
#include "RoomLog.h"
#include "../Event/Event.h"
#include "Sensor.h"
#include <DirectXMath.h>
#include "../Eventlog/LogAction.h"

using namespace DirectX;
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

struct RoomData
{
	XMFLOAT3 centerPosition;
	XMFLOAT3 size;
	float distanceToCorner;
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

	float CheckWorldRayCollision(const Ray &rRay);

	// Sensor specific
	std::vector<Event::Type> GetInputTypes() const;

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

	void InitRoomData(XMMATRIX matrix);

	// Returns a RoomInfo struct containing center position,
	// size and distance to corner.
	const RoomData GetRoomData() const;

	// Returns the room center position.
	const XMFLOAT3 GetRoomCenter() const;

	// Returns the distance from the center position
	// to the corner of the bounding box.
	const float GetDistanceToCorner() const;

	// Returns a vector containing the size of the 
	// bounding box in all directions.
	const XMFLOAT3 GetRoomSize() const;

private:

	// Room specific
	int mIndex_Boat;
	int mIndex_Deck;
	int mIndex_DeckInBoat;
	std::string mName;
	AABB mBoundingBox;

	AABB mWorldBoundingBox;

	// Deck specific
	std::string mDeckName;

	// Sensor specific
	Sensor mSensor;

	// Room info for camera purposes.
	RoomData mRoomData;

	// Log specific
	RoomLog mRoomLog;
};
