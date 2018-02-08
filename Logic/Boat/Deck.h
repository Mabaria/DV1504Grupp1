#pragma once

#include <string>
#include <vector>
#include "Room.h"

/**
*	Deck is a simple structure that holds offset and length of its own rooms in
* the Boat room list.
*/

class Deck
{
public:

	Deck();
	~Deck();
	
	// Deck specific
	void SetIndex(int index);
	void SetName(std::string name);
	std::string GetName() const;

	// Room specific
	void AddRoom(); // Increases room counter
	void SetRoomCount(int count);
	void SetRoomOffset(int index);
	void PushRoomOffset(int value = 1);
	int GetRoomCount() const;
	int GetRoomOffset() const;

	// Disk specific
	std::string GetString() const;

private:

	// Deck specific
	int mIndex;
	std::string mName;

	// Room specific
	int mRoomCount;
	int mRoomOffset;
};