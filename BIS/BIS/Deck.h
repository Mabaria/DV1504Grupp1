#pragma once

#include <vector>
#include "Room.h"

class Deck
{
public:

	Deck(std::string name);
	~Deck();
	
	void AddRoom();

	void SetName(std::string name);
	void SetRoomOffset(int index);
	void PushRoomOffset(int value = 1);

	std::string GetName() const;
	int GetRoomCount() const;
	int GetRoomOffset() const;

private:

	std::string mName;
	int mRoomCount;
	int mRoomOffset;
};