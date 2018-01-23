#include "Room.h"

Deck::Deck(std::string name)
{
	this->mName = name;
}

Deck::~Deck()
{
}

const std::string Deck::GetName() const
{
	return this->mName;
}

void Deck::AddRoom(std::name)
{
	this->mRooms.push_back(Room(name));
}
