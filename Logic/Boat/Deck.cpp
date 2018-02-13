#include "Deck.h"

Deck::Deck()
{
	this->mRoomCount = 0;
	this->mRoomOffset = 0;
}

Deck::Deck(DeckDesc desc)
{
	this->InitFromDesc(desc);
}

Deck::~Deck()
{
}



/**
*	Deck specific
*/

void Deck::SetIndex(int index)
{
	this->mIndex = index;
}

void Deck::SetName(std::string name)
{
	this->mName = name;
}

void Deck::InitFromDesc(DeckDesc desc)
{
	this->mName = desc.name;
	this->mIndex = desc.index;
	this->mRoomCount = 0;
	this->mRoomOffset = desc.roomOffset;
}

std::string Deck::GetName() const
{
	return this->mName;
}



/**
*	Room specific
*/

void Deck::AddRoom(Room *pRoom, int index)
{
	if (index == -1)
		this->mpRooms.push_back(pRoom);
	else
		this->mpRooms.insert(this->mpRooms.begin() + index, pRoom);

	this->mRoomCount++;
}

void Deck::SetRoomCount(int count)
{
	this->mRoomCount = count;
}

void Deck::SetRoomOffset(int index)
{
	this->mRoomOffset = index;
}

void Deck::PushRoomOffset(int value)
{
	this->mRoomOffset++;
}

int Deck::GetRoomCount() const
{
	return this->mRoomCount;
}

int Deck::GetRoomOffset() const
{
	return this->mRoomOffset;
}



/**
*	Disk specific
*/

std::string Deck::GetString() const
{
	std::string print = "";

	print += "d ";
	print += this->mName;

	return print;
}
