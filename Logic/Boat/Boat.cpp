#include "Boat.h"

Boat::Boat()
{
	this->mpEventLog = nullptr;
}

Boat::~Boat()
{
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		delete this->mpDecks[i];
	}

	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		delete this->mpRooms[i];
	}
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
	Deck *newDeck = new Deck;

	newDeck->SetName(name);
	newDeck->SetIndex(this->mpDecks.size());

	// Offset will be assigned.back() of list
	newDeck->SetRoomOffset((int)this->mpRooms.size());

	this->mpDecks.push_back(newDeck);
}

Deck* Boat::GetDeckPointer(std::string name)
{
	int index = this->GetDeckIndex(name);

	if (index != -1)
		return this->mpDecks[index];

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

	for (int i = 0; i < (int)this->mpDecks.size() && !deckFound; i++)
	{
		if (this->mpDecks[i]->GetName() == deckName)
		{
			Room *newRoom = new Room();

			newRoom->SetIndex((int)this->mpRooms.size());
			newRoom->SetName(roomName);
			newRoom->SetDeckName(deckName);
			
			for (int j = 0; j < (int)inputs.size(); j++)
			{
				newRoom->AddInputType(inputs[j]);
			}

			int offset = this->mpDecks[i]->GetRoomOffset() +
						 this->mpDecks[i]->GetRoomCount();

			if (this->mpEventLog != nullptr)
			{
				newRoom->SetEventLog(this->mpEventLog);
			}

			this->mpRooms.insert(this->mpRooms.begin() + offset, newRoom);
			this->mpDecks[i]->AddRoom(this->mpRooms[offset]);

			return;
		}
	}

	throw("Deck '" + deckName + "' not found");
}

Room* Boat::GetRoomPointer(std::string roomName, std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	if (index != -1)
		return this->mpRooms[index];
	return nullptr;
}

Room* Boat::GetPickedRoom(Ray ray)
{
	float tMain, t;
	//int hitIndex;
	Room* hitRoom = nullptr;

	tMain = -1; // Assume miss and prove collision
	//hitIndex = -1;

	// Check all rooms for collision
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		Ray wRay;
		wRay.origin = DirectX::XMVector3TransformCoord(
			ray.origin,
			this->mInverseFloorMatrix[i]);
		wRay.direction = DirectX::XMVector3TransformNormal(
			ray.direction,
			this->mInverseFloorMatrix[i]);
		for (int j = 0; j < this->mpDecks[i]->GetRoomCount(); j++)
		{
			t = this->mpDecks[i]->GetRoomPointerAt(j)->CheckRayCollision(wRay);


			if (
				(tMain == -1 && t >= 0) ||	// First hit
				(t >= 0 && t < tMain))	// Hit and closer than previous
			{
				tMain = t;
				hitRoom = this->mpDecks[i]->GetRoomPointerAt(j);
			}

		}
	}

	return hitRoom;

	//if (hitIndex != -1) // Hit found
	//{
	//	return this->mpRooms[hitIndex];
	//}

	//// No hit = return nullptr
	//return nullptr;
}




/**
*	Log specific
*/

void Boat::SetEventLog(EventLog *pEventLog)
{
	this->mpEventLog = pEventLog;

	for (int i = 0; i < (int)this->mpRooms.size(); i++)
	{
		this->mpRooms[i]->SetEventLog(pEventLog);
	}
}



/**
*	Event specific
*/

void Boat::CreateAutoEvent(Event::Type type,
	std::string roomName,
	std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mpRooms[index]->AddAutoEvent(type);
}

void Boat::CreatePlotEvent(Event::Type type,
	std::string roomName,
	std::string deckName)
{
	int index = this->GetRoomIndex(roomName, deckName);
	this->mpRooms[index]->AddPlotEvent(type);
}

void Boat::ClearEvent(Event::Type type,
	std::string roomName,
	std::string deckName)
{
	int roomIndex = this->GetRoomIndex(roomName, deckName);
	this->mpEventLog->ClearEvent(type, roomIndex);
}

std::vector<Event::Type> Boat::GetEventsInRoom(std::string roomName,
	std::string deckName)
{
	int roomIndex = this->GetRoomIndex(roomName, deckName);
	return this->mpEventLog->GetEvents(roomIndex);
}



/**
*	Disk specific
*/

void Boat::WriteFile(std::string filePath) const
{
	std::ofstream file;

	file.open(filePath);

	file << "boatmodel " << this->mModelName << "\n";

	file << "\n"; // Space
	
	file << "// d <<deck name>> <<offset>> <<count>>" << "\n";
	file << "// r <<room name>> / sensor <<room event index>>" << "\n\n";

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		Deck *pDeck = this->mpDecks[i];
		file << pDeck->GetString() << "\n";

		for (int j = pDeck->GetRoomOffset();
			j < pDeck->GetRoomOffset() + pDeck->GetRoomCount();
			j++)
		{
			file << this->mpRooms[j]->WriteString() << "\n";
		}

		file << "\n"; // Space
	}

	file.close();
}

bool Boat::ReadFile(std::string filePath)
{
	// Clear current lists
	this->mpDecks.clear();
	this->mpRooms.clear();

	std::ifstream file(filePath);

	std::stringstream buffer;
	std::string line;
	std::string word;

	DeckDesc dDesc;
	RoomDesc rDesc;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			buffer.clear();

			// Check first character of line
			switch (line[0])
			{
				case 'b': // Boat specific line
					this->mModelName = this->GetNameFromLine(line);

					break; // End case 'b'



				case 'd': // Deck specific line
					dDesc = this->FillDeckDescFromLine(line);
					dDesc.index = (int)this->mpDecks.size();
					dDesc.roomOffset = (int)this->mpRooms.size();

					/**
					*	Create a new deck and insert into the list
					*/
					this->mpDecks.push_back(new Deck(dDesc));

					break; // End case 'd'



				case 'r': // Room specific line
				{
					rDesc = this->FillRoomDescFromLine(line);

					rDesc.index = (int)this->mpRooms.size();
					rDesc.deckName = this->mpDecks.back()->GetName();
					rDesc.pEventLog = this->mpEventLog;

					/**
					*	Create a new room and insert into list
					*/
					this->mpRooms.push_back(new Room(rDesc));
					this->mpDecks.back()->AddRoom(this->mpRooms.back());

					break; // End case 'r'
				}

				default: // Comments
					break;
			}
		}

		// When.back() of file appears, close file and return from function
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

	int from = this->mpDecks[deckIndex]->GetRoomOffset();
	int to = from + this->mpDecks[deckIndex]->GetRoomCount();

	for (int i = from; i < to; i++)
	{
		if (this->mpRooms[i]->GetName() == roomName)
			return i;
	}

	return -1;
}

int Boat::GetDeckIndex(std::string deckName)
{
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		if (this->mpDecks[i]->GetName() == deckName)
			return i;
	}

	return -1;
}

RoomDesc Boat::FillRoomDescFromLine(std::string line)
{
	RoomDesc desc;

	std::stringstream buffer;
	std::string roomName, word;
	int number;

	buffer.str(line);

	/**
	*	Get room name
	*/
	roomName = "";
	buffer >> word; // Get 'r'
	buffer >> word;

	while (word != "/")
	{
		if (roomName != "")
		roomName += " ";
		roomName +=	word;

		buffer >> word;
	}
	desc.name = roomName;

	/**
	*	Get sensor data
	*/
	buffer >> word; // Get 'sensor'
	buffer >> number;	// roomEventIndex
	desc.activeIndex = number;

	buffer >> word; // Get rid of '{'

	while (buffer >> word)
	{
		if (word != "}")
		{
			number = std::stoi(word);
			desc.inputTypes.push_back((Event::Type)number); // Cast int to enum
		}
		else	// word == '}'
			break;
	}
	
	return desc;
}

DeckDesc Boat::FillDeckDescFromLine(std::string line)
{
	DeckDesc desc;
	desc.name = this->GetNameFromLine(line);

	return desc;
}

bool Boat::LoadBoundingBoxes(
	Mesh *meshList,
	DirectX::XMMATRIX **matrixList,
	int amount)
{
	// Sanity check, avoids out of bounds
	if (amount > (int)this->mpDecks.size())
		return false;

	int size;

	for (int i = 0; i < amount; i++)
	{
		this->mFloorMatrix.push_back(
			*matrixList[i]
		);
		this->mInverseFloorMatrix.push_back(
			DirectX::XMMatrixInverse(NULL, *matrixList[i]));

		std::vector<std::vector<Vertex>> &submeshList =
			meshList[i].GetVertexVectors();

		size = (int)submeshList.size();

		// Sanity check, avoids out of bounds
		if (this->mpDecks[i]->GetRoomOffset() + size >
			(int)this->mpRooms.size())
		{
			return false;
		}

		for (int j = 0; j < size; j++)
		{
			this->mpRooms[this->mpDecks[i]->GetRoomOffset() + j]->SetAABB(
				Picking::FromVerticesToAABB(submeshList[j]));			
		}
	}
	return true;
}

std::string Boat::GetNameFromLine(std::string line, char until)
{
	std::stringstream buffer;
	std::string name, word;
	bool done;

	name = "";
	done = false;
	buffer.str(line);

	buffer >> word; // Get line prefix
	buffer >> word; // Get first part of name (could be only word of name)

	while (!done)
	{
		if (name != "")
			name += " ";
		name +=	word;

		buffer >> word;
		
		if (until == NULL && buffer.eof())
			done = true;
		else if (word[0] == until)
			done = true;
	}

	return name;
}

std::string Boat::GetDeckNameByRoomIndex(int index)
{
	int count;
	int offset;

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		count = this->mpDecks[i]->GetRoomCount();
		offset = this->mpDecks[i]->GetRoomOffset();

		if (index >= offset && index <= (offset+count)) 
			return this->mpDecks[i]->GetName();
	}

	return "DeckNotFound";
}

