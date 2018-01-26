#include "Deck.h"

Deck::Deck()
{
	this->mRoomCount = 0;
	this->mRoomOffset = 0;
}

Deck::~Deck()
{
}

void Deck::AddRoom()
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
