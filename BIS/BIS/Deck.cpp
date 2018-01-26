#include "Room.h"

Deck::Deck(std::string name)
{
	this->mRoomCount = 0;
	this->mRoomOffset = 0;
}

Deck::~Deck()
{
}

const std::string Deck::GetName() const
{
	this->mRoomCount++;
}

void Deck::SetName(std::string name)
{
	this->mName = name;
}

void Deck::SetRoomOffset(int index)
{
	this->mRoomOffset = index;
}

void Deck::PushRoomOffset(int value)
{
	this->mRoomOffset++;
}

std::string Deck::GetName() const
{
	return this->mName;
}

int Deck::GetRoomCount() const
{
	return this->mRoomCount;
}

int Deck::GetRoomOffset() const
{
	return this->mRoomOffset;
}
