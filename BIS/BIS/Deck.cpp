#include "Deck.h"

Deck::Deck()
{
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

void Deck::SetRoomCap(int cap)
{
	this->mRoomCap = cap;
}

void Deck::SetRoomStartIndex(int index)
{
	this->mRoomStartIndex = index;
}

std::string Deck::GetName() const
{
	return this->mName;
}

int Deck::GetRoomCap() const
{
	return this->mRoomCap;
}

int Deck::GetRoomCount() const
{
	return this->mRoomCount;
}

int Deck::GetRoomStartIndex() const
{
	return this->mRoomStartIndex;
}
