#pragma once

#include <string>
#include "../../IO/Picking.h"
#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"
#include "Sensor.h"
#include <DirectXMath.h>

using namespace DirectX;

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

struct RoomData
{
	XMFLOAT3 centerPosition;
	XMFLOAT3 size;
	float distanceToCorner;
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

	float CheckWorldRayCollision(const Ray &rRay);

	// Sensor specific
	std::vector<Event::Type> GetInputTypes() const;

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
	int mIndexInBoat;
	int mIndexInDeck;
	int mDeckIndex;
	std::string mName;
	AABB mBoundingBox;

	AABB mWorldBoundingBox;

	// Deck specific
	std::string mDeckName;

	// Sensor specific
	Sensor mSensor;

	// Room info for camera purposes.
	RoomData mRoomData;
};
