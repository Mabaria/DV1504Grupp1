#include "Boat.h"

Boat::Boat()
{
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
*	Build boat structure
*/

void Boat::AddDeck(std::string name)
{
	Deck newDeck;

	newDeck.SetName(name);

	// Offset will be assigned end of list
	newDeck.SetRoomOffset((int)this->mRooms.size());

	this->mDecks.push_back(newDeck);
}

void Boat::AddRoom(std::string roomName, std::string deckName, int inputs[NR_OF_EVENT_TYPES])
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
			
			for (int j = 0; j < (sizeof(inputs)/sizeof(int)); j++)
			{
				newRoom.AddInputType((Event::Type)inputs[j]);
			}

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



/**
*	Log specific
*/

void Boat::SetEventLog(EventLog *pEventLog)
{
	for (int i = 0; i < this->mRooms.size(); i++)
	{
		this->mRooms[i].SetEventLog(pEventLog);
	}
}

// TODO: SetActiveEventIndex

/**
*	Event specific
*/

void Boat::CreateEvent(Event::Type type, std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mRooms[index].AddEvent(type);
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
					break;

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
						for (int i = 2; i < word.size(); i++)
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
					break;

				case 'r': // Room specific line
					buffer.str(line);	// Fill buffer with line
					buffer >> word;

					if (word[1] == '#') // room line
					{
						Room newRoom;

						/**
						*	Get index
						*/
						std::stringstream intParse;
						for (int i = 2; i < word.size(); i++)
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
							*	Insert room into list
							*/
						this->mRooms.push_back(newRoom);
					}

					else if (word[1] == 'o')
					{
						if (buffer >> number)
							this->mRooms.reserve(number);
						else
							throw ("Number missing after 'roomcount'.");

					}
					break;

				default: // Comments
					break;
			}
		}

		// When end of file appears, close file and return from function
		file.close();
		return;
	}

	// If file could not be opened
	throw("Can't open file '" + filePath + "', file not found");
}



/**
*	Private functions to Boat
*/

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

