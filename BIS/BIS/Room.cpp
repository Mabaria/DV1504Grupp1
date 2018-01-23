#include "Room.h"

Room::Room(std::string name, std::string deckName)
{
	this->mName = name;
	this->mDeckName = deckName;
}

Room::~Room()
{

}

const bool Room::HasEvents() const
{
	return this->mEvents.size() > 0;
}

// Returns true if successful
const bool Room::AddEvent(EventType type)
{
	
}

// Returns true if successful
const bool Room::AddEvent(EventType type)
{
}

const std::string Room::GetName() const
{
	return this->mName;
}

const std::string Room::GetDeck() const
{
	return this->mDeckName;
}
