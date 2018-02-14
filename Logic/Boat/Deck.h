#pragma once

#include <string>
#include <vector>
#include "Room.h"

/**
*	Deck is a simple structure that holds offset and length of its own rooms in
* the Boat room list.
*/

struct DeckDesc
{
	int index;
	int roomOffset;
	std::string name;
};

class Deck
{
public:

	Deck();
	Deck(DeckDesc desc);
	~Deck();
	
	// Deck specific
	void SetIndex(int index);
	void SetName(std::string name);
	void InitFromDesc(DeckDesc desc);
	std::string GetName() const;

	// Room specific
	void AddRoom(Room *pRoom, int index = -1);
	void SetRoomCount(int count);
	void SetRoomOffset(int index);
	void PushRoomOffset(int value = 1);
	int GetRoomCount() const;
	int GetRoomOffset() const;
	Room* GetRoomPointerAt(int index);

	// Disk specific
	std::string GetString() const;

private:

	// Deck specific
	int mIndex;
	std::string mName;

	// Room specific
	int mRoomCount;
	int mRoomOffset;
	std::vector<Room*> mpRooms;
};
