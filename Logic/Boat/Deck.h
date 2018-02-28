#pragma once

#include <string>
#include <vector>
#include "Room.h"
#include "../Eventlog/EventLog.h"

/**
*	Deck is a simple structure that holds offset and length of its own rooms in
* the Boat room list.
*/

struct Desc_Deck
{
	int index;
	int roomOffset;
	std::string name;
	EventLog *pEventLog;
};


class Deck
{
public:

	Deck();
	Deck(Desc_Deck desc);
	~Deck();
	
	// Deck specific
	void SetIndex(int index);
	void SetName(std::string name);
	void InitFromDesc(Desc_Deck desc);
	std::string GetName() const;

	// Room specific
	bool AddRoom(std::string name, int inputs);
	bool AddRoom(std::string line);
	void SetRoomOffset(int index);
	int GetRoomCount() const;
	int GetRoomOffset() const;
	int GetRoomIndex_Boat(std::string name) const;
	int GetRoomIndex_Deck(std::string name) const;
	Room* GetRoomPointer(std::string name) const;
	Room* GetRoomPointerAt(int index) const;

	// Disk specific
	std::string GetString() const;
	std::string GetRoomStringAt(int index) const;
	static Desc_Deck FillDeckDescFromLine(std::string line);

private:

	// Deck specific
	int mIndex;
	std::string mName;

	// Room specific
	int mRoomOffset;
	std::vector<Room*> mpRooms;
	
	// Log specific
	EventLog *mpEventLog;
};

