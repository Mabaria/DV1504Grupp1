#include "Boat.h"

Boat::Boat()
{
}

Boat::~Boat()
{
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		delete this->mpDecks[i];
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
	Desc_Deck desc;

	int offset = 0;
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
		offset += this->mpDecks[i]->GetRoomCount();

	desc.index = this->mpDecks.size();
	desc.roomOffset = offset;
	desc.name = name;
	desc.pEventLog = &this->mEventLog;

	this->mpDecks.push_back(new Deck(desc));
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

bool Boat::AddRoom(std::string roomName,
	std::string deckName,
	int inputs)
{
	// Find deck
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		if (this->mpDecks[i]->GetName() == deckName)
		{
			// Create room in deck
			return this->mpDecks[i]->AddRoom(roomName, inputs);
		}
	}
	
	// Deck not found
	return false;
}

Room* Boat::GetRoomPointer(std::string name)
{
	Room *pRoom;

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		pRoom = this->mpDecks[i]->GetRoomPointer(name);

		if (pRoom != nullptr)
			return pRoom;
	}

	// Room not found
	return nullptr;
}

Room* Boat::GetRoomPointerAt(int index_Boat)
{
	if (index_Boat < 0 || index_Boat > (int)this->GetRoomCount())
		return nullptr;
	
	// Find deck
	for (int i = (int)this->mpDecks.size(); i > 0; i--)
	{
		if (index_Boat >= this->mpDecks[i]->GetRoomOffset())
		{
			return this->mpDecks[i]->GetRoomPointerAt(
				index_Boat - this->mpDecks[i]->GetRoomOffset());
		}
	}
	
	// If not found
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
}

int Boat::GetRoomCount() const
{
	int count = 0;

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
		count += this->mpDecks[i]->GetRoomCount();
	
	return count;
}



/**
*	Log specific
*/

int Boat::GetEventCount() const
{
	return this->mEventLog.GetEventCount();
}

int Boat::GetActionCount() const
{
	return this->mEventLog.GetActionCount();
}



/**
*	Disk specific
*/

void Boat::SaveToFile_Boat(std::string filePath) const
{
	std::ofstream file;

	file.open(filePath);

	file << "boatmodel " << this->mModelName << "\n";

	file << "\n"; // Space
	
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		Deck *pDeck = this->mpDecks[i];
		file << pDeck->GetString() << "\n";

		for (int i = 0; i < pDeck->GetRoomCount(); i++)
			file << pDeck->GetRoomStringAt(i) << "\n";

		file << "\n"; // Space
	}

	file.close();
}

bool Boat::LoadFromFile_Boat(std::string filePath)
{
	// Clear current decks
	for (int i = 0; i < (int)this->mpDecks.size(); i++)
		delete this->mpDecks[i];
	this->mpDecks.clear();

	std::ifstream file(filePath);
	std::string line;
	
	if (file.is_open())
	{
		while (getline(file, line))
		{
			// Check first character of line
			switch (line[0])
			{
				case 'b': // Boat specific line
				{					
					std::stringstream ss;
					std::string word;

					std::string name;
					ss.str(line);

					ss >> word; // Get 'b'
					ss >> name; // Get first part of name (could be only word of name)

					while (!ss.eof())
					{
						ss >> word;
						if (name != "")
							name += " ";
						name += word;
					}

					this->mModelName = Room::CorrectName(name);

					break; // End case 'b'
				}



				case 'd': // Deck specific line
				{
					Desc_Deck desc;
					
					desc = Deck::FillDeckDescFromLine(line);
					
					int roomOffset = 0;
					
					for (int i = 0; i < (int)this->mpDecks.size(); i++)
						roomOffset += this->mpDecks[i]->GetRoomOffset();


					desc.index = (int)this->mpDecks.size();
					desc.roomOffset = roomOffset;
					desc.pEventLog = &this->mEventLog;


					/**
					*	Create a new deck and insert into the list
					*/
					this->mpDecks.push_back(new Deck(desc));

					break; // End case 'd'
				}


				case 'r': // Room specific line
				{
					this->mpDecks.back()->AddRoom(line);
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

void Boat::SaveToFile_Log(std::string filePath) const
{
	this->mEventLog.SaveToFile(filePath);
}

bool Boat::LoadFromFile_Log(std::string filePath)
{
	return this->mEventLog.LoadFromFile(filePath);
}



/**
*	Private functions to Boat
*/

int Boat::GetRoomIndex_Boat(std::string roomName)
{
	int index;

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		index = this->mpDecks[i]->GetRoomIndex_Boat(roomName);

		if (index != -1)
			return index;
	}

	// If not found
	return -1;
}

int Boat::GetRoomIndex_Deck(std::string roomName)
{	
	int index;

	for (int i = 0; i < (int)this->mpDecks.size(); i++)
	{
		index = this->mpDecks[i]->GetRoomIndex_Deck(roomName);

		if (index != -1)
			return index;
	}

	// If not found
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
			(int)this->GetRoomCount())
		{
			return false;
		}

		for (int j = 0; j < size; j++)
		{
//			this->mpRooms[this->mpDecks[i]->GetRoomOffset() + j]->SetAABB(
//				Picking::FromVerticesToAABB(submeshList[j]));			
			this->mpDecks[i]->GetRoomPointerAt(j)->SetAABB(
				Picking::FromVerticesToAABB(submeshList[j]));
		}
	}
	return true;
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
