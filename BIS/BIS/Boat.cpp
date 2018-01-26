#include "Boat.h"

Boat::Boat()
{
}

Boat::~Boat()
{
}

void Boat::CreateEvent(Event::Type type, std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mRooms[index].AddEvent(type);
}

void Boat::AddDeck(std::string name)
{
	Deck newDeck;

	newDeck.SetName(name);

	// Offset will be assigned end of list
	newDeck.SetRoomOffset((int)this->mRooms.size());

	this->mDecks.push_back(newDeck);
}

void Boat::AddRoom(std::string roomName, std::string deckName)
{
	// Check early exit
	if (this->GetRoomIndex(roomName, deckName) != -1)
		throw("Room '" + roomName + "' already exist");

	bool deckFound = false;

	for (int i = 0; i < this->mDecks.size() && !deckFound; i++)
	{
		if (this->mDecks[i].GetName() == deckName)
		{
			Room newRoom;

			newRoom.SetIndex((int)this->mRooms.size());
			newRoom.SetName(roomName);
			newRoom.SetDeckName(deckName);

			int offset = this->mDecks[i].GetRoomOffset() +
						 this->mDecks[i].GetRoomCount();

			this->mRooms.insert(this->mRooms.begin() + offset, newRoom);
			this->mDecks[i].AddRoom();

			// Add +1 to remaining decks offset after this deck, if any
			for (int j = i+1; j < this->mDecks.size(); j++)
			{
				this->mDecks[j].PushRoomOffset(); // Push 1 step to the right
			}

			return;
		}
	}

	throw("Deck '" + deckName + "' not found");
}

std::string Boat::GetModelName() const
{
	return this->mModelName;
}

void Boat::SetModelName(std::string name)
{
	this->mModelName = name;
}

void Boat::SetEventLog(EventLog *pEventLog)
{
	for (int i = 0; i < this->mRooms.size(); i++)
	{
		this->mRooms[i].SetEventLog(pEventLog);
	}
}

int Boat::GetRoomIndex(std::string roomName)
{
	for (int i = 0; i < this->mRooms.size(); i++)
	{
		if (this->mRooms[i].GetName() == roomName)
			 return i;
	}

	return -1;
}

int Boat::GetRoomIndex(std::string roomName, std::string deckName)
{
	int deckIndex = this->GetDeckIndex(deckName);

	// Check early exit
	if (deckIndex == -1)
		return -1;

	int from = this->mDecks[deckIndex].GetRoomOffset();
	int to = from + this->mDecks[deckIndex].GetRoomCount();

	for (int i = from; i < to; i++)
	{
		if (this->mRooms[i].GetName() == roomName)
			return i;
	}

	return -1;
}

int Boat::GetDeckIndex(std::string deckName)
{
	for (int i = 0; i < this->mDecks.size(); i++)
	{
		if (this->mDecks[i].GetName() == deckName)
			return i;
	}

	return -1;
}

std::string Boat::GetDeckNameByRoomIndex(int index)
{
	int count;
	int offset;

	for (int i = 0; i < this->mDecks.size(); i++)
	{
		count = this->mDecks[i].GetRoomCount();
		offset = this->mDecks[i].GetRoomOffset();

		if (index >= offset && index <= (offset+count)) 
			return this->mDecks[i].GetName();
	}

	return "DeckNotFound";
}

void Boat::WriteFile(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);
	
	file << "boatmodel " << this->mModelName << "\n";

	file << "\n"; // Space
	
	file << "// index deck name" << "\n";
	file << "deckcount " << this->mDecks.size() << "\n";
	for (int i = 0; i < this->mDecks.size(); i++)
		file << "d#" << i << " " << this->mDecks[i].GetName() << "\n";

	file << "\n"; // Space

	file << "// r#index <<deck name>> / <<room name>> / sensor <<room event index>>" << "\n";
	file << "roomcount " << this->mRooms.size() << "\n";
	for (int i = 0; i < this->mRooms.size(); i++)
	{
		file << this->mRooms[i].WriteString() << "\n";
	}

	file.close();
}

void Boat::ReadFile(std::string filePath)
{
}
