#pragma once

#include <vector>
#include "Room.h"

class Deck
{
public:

	Deck(std::string name);
	~Deck();
	
	const std::string GetName() const;
	void AddRoom(std::name);

private:

	string mName;
	std::vector<Room> mRooms;
};
