#include "Deck.h"

Deck::Deck()
{
	mpEventLog = nullptr;
}

Deck::Deck(Desc_Deck desc)
{
	this->InitFromDesc(desc);
}

Deck::~Deck()
{
	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		delete this->mpRooms[i];
	}
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

void Deck::InitFromDesc(Desc_Deck desc)
{
	this->mIndex = desc.index;
	this->mRoomOffset = desc.roomOffset;
	this->mName = desc.name;
	this->mpEventLog = desc.pEventLog;
}

std::string Deck::GetName() const
{
	return this->mName;
}



/**
*	Room specific
*/

bool Deck::AddRoom(std::string name, int inputs)
{
	Desc_Room desc;
	
	desc.inputs = inputs;
	desc.index_Boat = (int)this->mpRooms.size() + this->mRoomOffset;
	desc.index_Deck = (int)this->mpRooms.size();
	desc.index_DeckInBoat = this->mIndex;
	desc.name = name;
	desc.deckName = this->mName;
	desc.pEventLog = this->mpEventLog;

	// Create new room
	this->mpRooms.push_back(new Room(desc));
	return true;
}

bool Deck::AddRoom(std::string line)
{
	Desc_Room desc = Room::FillRoomDescFromLine(line);

	desc.index_Boat = this->mIndex + (int)this->mpRooms.size();
	desc.index_Deck = (int)this->mpRooms.size();
	desc.index_DeckInBoat = this->mIndex;
	desc.deckName = this->mName;
	desc.pEventLog = this->mpEventLog;

	this->mpRooms.push_back(new Room(desc));
	return true;
}

int Deck::GetRoomIndex_Boat(std::string name) const
{
	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		if (this->mpRooms[i]->GetName() == name)
			return this->mpRooms[i]->GetIndex_Boat();
	}

	return -1;
}

int Deck::GetRoomIndex_Deck(std::string name) const
{
	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		if (this->mpRooms[i]->GetName() == name)
			return i;
	}

	return -1;
}

Room* Deck::GetRoomPointer(std::string name) const
{
	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		if (this->mpRooms[i]->GetName() == name)
			return this->mpRooms[i];
	}

	return nullptr;
}

Room* Deck::GetRoomPointerAt(int index) const
{
	if (index < 0 || index >= (int)this->mpRooms.size())
		return nullptr;
	return this->mpRooms[index];
}
void Deck::SetRoomOffset(int index)
{
	this->mRoomOffset = index;
}

int Deck::GetRoomCount() const
{
	return (int)this->mpRooms.size();
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

std::string Deck::GetRoomStringAt(int index) const
{
	if (index < 0 || index >= (int)this->mpRooms.size())
		return "";
	return this->mpRooms[index]->GetString();
}

Desc_Deck Deck::FillDeckDescFromLine(std::string line)
{
	Desc_Deck desc;

	std::stringstream ss(line);
	std::string name, word;

	ss >> word; // Get 'd'
	ss >> name; // Get first part of name (could be only word of name)

	while (!ss.eof())
	{
		ss >> word;
		if (name != "")
			name += " ";
		name += word;
	}
	desc.name = Room::CorrectName(name);
	return desc;
}
