#pragma once

#include <string>
#include <vector>
#include "Deck.h"
#include "Room.h"

class Boat
{
public:

	Boat();
	~Boat();

	void AddDeck(std::string name, int roomCount);
	void AddRoom(std::string roomName, std::string deckName);
	void SetRoomEventIndex(std::string roomName, std::string deckName, int index);

private:	

	std::vector<Deck> mDecks;
	std::vector<Room> mRooms;
};
