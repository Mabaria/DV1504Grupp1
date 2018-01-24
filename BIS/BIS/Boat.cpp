#include "Boat.h"

Boat::Boat()
{
}

Boat::~Boat()
{
}

void Boat::AddDeck(std::string name, int roomCount)
{
	Deck newDeck;

	newDeck.SetName(name);
	newDeck.SetRoomCount(roomCount);
	newDeck.SetRoomStartIndex((int)this->mRooms.size());

	this->mDecks.push_back(newDeck);
}

void SetRoomName(std::string roomName, std::string deckName)
{
	for (int i = 0; i < this->mDecks.size(); i++)
	{
		if (this->mDecks[i].GetName() == deckName)
		{
			Room newRoom;

			newRoom.SetName(roomName);

			// TODO: Change from vector???
			this->mRooms.insert(
				this->mDecks[i].GetRoomStartIndex() + this->mDecks[i].GetRoomCount(),
				newRoom);
		}
	}
}
