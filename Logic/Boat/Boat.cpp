#include "Boat.h"

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

Deck* Boat::GetDeckPointerAt(int index)
{
	// Sanity check
	if (index >= 0 && index < (int)this->mDecks.size())
	{
		return &this->mDecks[index];
	}

	// If invalid index
	return nullptr;
}



/**
*	Room specific
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
			this->mDecks[i].AddRoom();

			// Add +1 to remaining decks offset after this deck, if any
			for (int j = i+1; j < (int)this->mDecks.size(); j++)
			{
				this->mDecks[j].PushRoomOffset(); // Push 1 step to the right
			}

			return;
		}
	}

	throw("Deck '" + deckName + "' not found");
}

Room* Boat::GetPickedRoom(Ray ray)
{
	float hitIndex, tMain, t;

	tMain = -1; // Assume miss and prove collision
	hitIndex = -1;

	// Check all rooms for collision
	for (int i = 0; i < (int)this->mBoundingAABB.size(); i++)
	{
		//t = this->mRooms[i].CheckRayCollision(ray);
		t = Picking::IsRayIntersectingAABB(ray, this->mBoundingAABB[i]);

		if (
			(tMain == -1 && t >= 0) ||	// First hit
			(t >= 0 && t < tMain))	// Hit and closer to the "eye" than previous room
		{
			tMain = t;
			hitIndex = i;
		}
	}

	if (hitIndex != -1) // Hit found
	{
		return &this->mRooms[hitIndex];
	}

	// No hit = return nullptr
	return nullptr;
}

Room* Boat::GetRoomPointerAt(int index)
{
	// Sanity check
	if (index >= 0 && index < (int)this->mRooms.size())
		return &this->mRooms[index];

	// If invalid index
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

void Boat::WriteFile(std::string filePath) const
{
	std::ofstream file;

	file.open(filePath);

	file << "boatmodel " << this->mModelName << "\n";

	file << "\n"; // Space
	
	file << "// d#index <<deck name>> <<offset>> <<count>>" << "\n";
	file << "deckcount " << this->mDecks.size() << "\n";

	for (int i = 0; i < (int)this->mDecks.size(); i++)
		file << this->mDecks[i].GetString() << "\n";

	file << "\n"; // Space

	file << "// r#index <<deck name>> / <<room name>> / sensor <<room event index>>" << "\n";
	file << "roomcount " << this->mRooms.size() << "\n";
	for (int i = 0; i < (int)this->mRooms.size(); i++)
	{
		file << this->mRooms[i].WriteString() << "\n";
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

					if (word[1] == '#') // room line
					{
						Room newRoom;

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
					}

					else if (word[1] == 'o')
					{
						if (buffer >> number)
							this->mRooms.reserve(number);
						else
							throw ("Number missing after 'roomcount'.");

					}
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

bool Boat::LoadBoundingBoxes()
{
	//? Is there any point in not hardcode the files?
	//Todo Need to try-catch this
	this->mBoundingMesh.push_back(
		Mesh("../../Models/Bounding/Bound01UV.obj"));
	this->mBoundingMesh.push_back(
		Mesh("../../Models/Bounding/Bound1UV.obj"));
	this->mBoundingMesh.push_back(
		Mesh("../../Models/Bounding/Bound2UV.obj"));

	this->mBoundingMeshObjects.push_back(MeshObject(
		"Bound01",
		this->mBoundingMesh[0].GetIndexVectors(),
		this->mBoundingMesh[0].GetVertexVectors()
	));
	this->mBoundingMeshObjects.push_back(MeshObject(
		"Bound1",
		this->mBoundingMesh[1].GetIndexVectors(),
		this->mBoundingMesh[1].GetVertexVectors()
	));
	this->mBoundingMeshObjects.push_back(MeshObject(
		"Bound2",
		this->mBoundingMesh[2].GetIndexVectors(),
		this->mBoundingMesh[2].GetVertexVectors()
	));

	for (int i = 0; i < this->mBoundingMesh.size(); i++)
	{
		std::vector<std::vector<Vertex>> &submeshList =
			this->mBoundingMesh[i].GetVertexVectors();
		for (int j = 0; j < submeshList.size(); j++)
		{
			this->mBoundingAABB.push_back(
				Picking::FromMeshToAABB(submeshList[j]));
			
		}
	}
	return true;
}

MeshObject *Boat::GetBoundingMeshObject(int index)
{
	if (index < 0 || index >= this->mBoundingMeshObjects.size())
		return nullptr;
	return &this->mBoundingMeshObjects[index];
}

bool Boat::LoadBoatMesh()
{
	this->mDeckMesh.push_back(Mesh("../../Models/DeckMesh/Floor01.obj"));
	this->mDeckMesh.push_back(Mesh("../../Models/DeckMesh/Floor1.obj"));
	this->mDeckMesh.push_back(Mesh("../../Models/DeckMesh/Floor2.obj"));

	this->mDeckMeshObject.push_back(MeshObject(
		"Floor01",
		this->mDeckMesh[0].GetIndexVectors(),
		this->mDeckMesh[0].GetVertexVectors()
	));
	this->mDeckMeshObject.push_back(MeshObject(
		"Floor1",
		this->mDeckMesh[1].GetIndexVectors(),
		this->mDeckMesh[1].GetVertexVectors()
	));
	this->mDeckMeshObject.push_back(MeshObject(
		"Floor2",
		this->mDeckMesh[2].GetIndexVectors(),
		this->mDeckMesh[2].GetVertexVectors()
	));
	return true;
}

MeshObject *Boat::GetDeckMeshObject(int index)
{
	if (index < 0 || index >= this->mDeckMeshObject.size())
		return nullptr;
	return &this->mDeckMeshObject[index];
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

