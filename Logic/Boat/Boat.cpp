#include "Boat.h"

/**
*	TODO
*		Varje deck läses in för sig, där deras rum raddas upp efter däcknamnet. Fixa ReadFile för room-line.
*/

Boat::Boat()
{
	this->mpEventLog = nullptr;
}

Boat::~Boat()
{
}



/**
*	Boat specific
*/

std::string Boat::GetModelName() const
{
	return this->mModelName;
}

void Boat::SetModelName(std::string name)
{
	this->mModelName = name;
}



/**
*	Deck specific
*/

void Boat::AddDeck(std::string name)
{
	Deck newDeck;

	newDeck.SetName(name);
	newDeck.SetIndex(this->mDecks.size());

	// Offset will be assigned end of list
	newDeck.SetRoomOffset((int)this->mRooms.size());

	this->mDecks.push_back(newDeck);
}

Deck* Boat::GetDeckPointer(std::string name)
{
	int index = this->GetDeckIndex(name);

	if (index != -1)
		return &this->mDecks[index];

	return nullptr;
}



/**
* Room specific
*/

void Boat::AddRoom(std::string roomName,
	std::string deckName,
	std::vector<Event::Type> inputs)
{
	// Check early exit
	if (this->GetRoomIndex(roomName, deckName) != -1)
		throw("Room '" + roomName + "' already exist");

	bool deckFound = false;

	for (int i = 0; i < (int)this->mDecks.size() && !deckFound; i++)
	{
		if (this->mDecks[i].GetName() == deckName)
		{
			Room newRoom;

			newRoom.SetIndex((int)this->mRooms.size());
			newRoom.SetName(roomName);
			newRoom.SetDeckName(deckName);
			
			for (int j = 0; j < (int)inputs.size(); j++)
			{
				newRoom.AddInputType(inputs[j]);
			}

			int offset = this->mDecks[i].GetRoomOffset() +
						 this->mDecks[i].GetRoomCount();

			if (this->mpEventLog != nullptr)
			{
				newRoom.SetEventLog(this->mpEventLog);
			}

			this->mRooms.insert(this->mRooms.begin() + offset, newRoom);
			this->mDecks[i].AddRoom(&this->mRooms[offset]);

			return;
		}
	}

	throw("Deck '" + deckName + "' not found");
}

Room* Boat::GetRoomPointer(std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);

	if (index != -1)
		return &this->mRooms[index];

	return nullptr;
}



/**
*	Log specific
*/

void Boat::SetEventLog(EventLog *pEventLog)
{
	this->mpEventLog = pEventLog;

	for (int i = 0; i < (int)this->mRooms.size(); i++)
	{
		this->mRooms[i].SetEventLog(pEventLog);
	}
}

/**
*	Event specific
*/

void Boat::CreateAutoEvent(Event::Type type, std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mRooms[index].AddAutoEvent(type);
}

void Boat::CreatePlotEvent(Event::Type type, std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mRooms[index].AddPlotEvent(type);
}

void Boat::ClearEvent(Event::Type type, std::string roomName, std::string deckName)
{
	int roomIndex = this->GetRoomIndex(roomName, deckName);
	this->mpEventLog->ClearEvent(type, roomIndex);
}

std::vector<Event::Type> Boat::GetEventsInRoom(std::string roomName, std::string deckName)
{
	int roomIndex = this->GetRoomIndex(roomName, deckName);
	return this->mpEventLog->GetEvents(roomIndex);
}



/**
*	Disk specific
*/

void Boat::WriteFile(std::string filePath)
{
	std::ofstream file;

	file.open(filePath);

	file << "boatmodel " << this->mModelName << "\n";

	file << "\n"; // Space
	
	file << "// d <<deck name>> <<offset>> <<count>>" << "\n";
	file << "// r <<room name>> / sensor <<room event index>>" << "\n\n";

	for (int i = 0; i < (int)this->mDecks.size(); i++)
	{
		Deck *pDeck = &this->mDecks[i];
		file << pDeck->GetString() << "\n";

		for (int j = pDeck->GetRoomOffset();
			j < pDeck->GetRoomOffset() + pDeck->GetRoomCount();
			j++)
		{
			file << this->mRooms[j].WriteString() << "\n";
		}

		file << "\n"; // Space
	}

	file.close();
}

bool Boat::ReadFile(std::string filePath)
{
	// Clear current lists
	this->mDecks.clear();
	this->mRooms.clear();

	std::ifstream file(filePath);

	std::stringstream buffer;
	std::string line;
	std::string word;
	int number;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			buffer.clear();

			// Check first character of line
			switch (line[0])
			{
				case 'b': // Boat specific line
					buffer.str(line);	// Fill buffer with line

					buffer >> word;
					if (word == "boatmodel")
					{
						if (buffer >> word)
							this->mModelName = word;
						else // No following word
							throw ("Boatmodel missing after 'boatmodel'.");
					}
					break; // End case 'b'



				case 'd': // Deck specific line
					buffer.str(line);	// Fill buffer with line

					buffer >> word; // Get first word
					if (word[1] == '#') // deck line
					{
						Deck newDeck;

						/**
						*	Get index
						*/
						std::stringstream intParse;
						for (int i = 2; i < (int)word.size(); i++)
						{
							intParse << word[i];
						}
						intParse >> number;
						newDeck.SetIndex(number);

						/**
						*	Get deck name
						*/
						buffer >> word;
						newDeck.SetName(word);

						/**
						*	Get room offset
						*/
						buffer >> number;
						newDeck.SetRoomOffset(number);

						/**
						*	Get room count
						*/
						buffer >> number;
						newDeck.SetRoomCount(number);
						
						/**
						*	Insert deck into list
						*/
						this->mDecks.push_back(newDeck);
					}

					else if (word[1] == 'e')	// deckcount
					{
						if (buffer >> number)
							this->mDecks.reserve(number);
						else // No following number
							throw ("Number missing after 'deckcount'.");
					}
					break; // End case 'd'



				case 'r': // Room specific line
					buffer.str(line);	// Fill buffer with line
					buffer >> word;

					Room newRoom;

					std::string roomName;
					std::string deckName;

					/**
					*	Get index
					*/
					std::stringstream intParse;
					for (int i = 2; i < (int)word.size(); i++)
					{
					intParse << word[i];
					}
					intParse >> number;

					newRoom.SetIndex(number);
					
					/**
					*	Get deck name
					*/
					buffer >> word; // Deck name
					newRoom.SetDeckName(word);
					deckName = word;
					
					int deckIndex = this->GetDeckIndex(word);

					// (Reads in automaticaly from file)
					//this->mDecks[deckIndex].AddRoom();
					//for (int i = deckIndex; i < this->mDecks.size(); i++)
					//{
					//	this->mDecks[i].PushRoomOffset();
					//}

					/**
					*	Get room name
					*/
					buffer >> word;	// Get rid of '/'

					std::string tempName = "";

					buffer >> word;

					while (word != "/")
					{
					if (tempName != "")
					tempName += " ";
					tempName +=	word;

					buffer >> word;
					}

					newRoom.SetName(tempName);
					roomName = tempName;

					/**
					*	Get sensor data
					*/
					buffer >> word; // 'sensor'
					buffer >> number;	// roomEventIndex
					newRoom.SetActiveEventIndex(number);

					buffer >> word; // Get rid of '{'

					while (buffer >> word)
					{
					if (word != "}")
					{
					number = std::stoi(word);
					newRoom.AddInputType((Event::Type)number); // Cast int to enum
					}
					else	// word == '}'
					break;
					}

					/**
					*	Give the room an eventlog pointer, if possible
					*/
					if (this->mpEventLog != nullptr)
					newRoom.SetEventLog(this->mpEventLog);

					/**
					*	Insert room into list
					*/
					this->mRooms.push_back(newRoom);

					/**
					*	Send room pointer to deck
					*/
					Deck* deck = &this->mDecks[deckIndex];
					// TODO
					int roomDeckIndex = deck->GetRoomCount();

					break; // End case 'r'

				default: // Comments
					break;
			}
		}

		// When end of file appears, close file and return from function
		file.close();
	}
	else
	{
		return false;
	}

	// Everything is successful
	return true;
}



/**
*	Private functions to Boat
*/

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
	for (int i = 0; i < (int)this->mDecks.size(); i++)
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

	for (int i = 0; i < (int)this->mDecks.size(); i++)
	{
		count = this->mDecks[i].GetRoomCount();
		offset = this->mDecks[i].GetRoomOffset();

		if (index >= offset && index <= (offset+count)) 
			return this->mDecks[i].GetName();
	}

	return "DeckNotFound";
}

