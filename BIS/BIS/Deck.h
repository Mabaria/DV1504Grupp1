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

	void SetName(std::string name);
	void SetRoomCap(int cap);
	void SetRoomStartIndex(int index);

	std::string GetName() const;
	int GetRoomCap() const;
	int GetRoomCount() const;
	int GetRoomStartIndex() const;

private:

	std::string mName;
	int mRoomCap;
	int mRoomCount;
	int mRoomStartIndex;
};
