#pragma once

#include <string>
#include <vector>
#include "Room.h"

class Deck
{
public:

	Deck();
	~Deck();
	
	void AddRoom();

	void SetIndex(int index);
	void SetName(std::string name);
	void SetRoomCount(int count);
	void SetRoomOffset(int index);
	void PushRoomOffset(int value = 1);

	std::string GetName() const;
	int GetRoomCount() const;
	int GetRoomOffset() const;

private:

	// Name and index of this deck
	int mIndex;
	std::string mName;

	int mRoomCount;
	int mRoomOffset;
};
