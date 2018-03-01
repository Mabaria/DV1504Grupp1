#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Deck.h"
#include "Room.h"

#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"

#include "../../IO/Picking.h"
#include "../../UI/UIElements/MeshObject.h"

class Boat
{
public:

	Boat();
	~Boat();
	
	// Boat specific
	std::string GetModelName() const;
	void SetModelName(std::string name);

	// Deck specific
	void AddDeck(std::string name);
	Deck* GetDeckPointer(std::string name);

	// Room specific
	bool AddRoom(std::string roomName,
		std::string deckName,
		int inputs);
	Room* GetRoomPointer(std::string name);
	Room* GetRoomPointerAt(int index_Boat);
	Room* GetPickedRoom(Ray ray);
	int GetRoomCount() const;

	// Same as GetPickedRoom() although it returns the position in which
	// the click hits as an XMFLOAT3 if it hits a room.
	// No hit returns {-100, -100, -100}.
	XMFLOAT3 GetPickedPosition(Ray ray);

	// Log specific
	int GetEventCount() const;
	int GetActionCount() const;

	// Disk specific
	void SaveToFile_Boat(std::string filePath) const;
	bool LoadFromFile_Boat(std::string filePath);

	void SaveToFile_Log(std::string filePath) const;
	bool LoadFromFile_Log(std::string filePath);

	// Room data specific
	bool LoadBoundingBoxes(
		Mesh *meshList,
		DirectX::XMMATRIX **matrixList,
		int amount);

private:
	
	// Returns -1 if item is not found
	int GetRoomIndex_Boat(std::string roomName);
	int GetRoomIndex_Deck(std::string roomName);
	int GetDeckIndex(std::string deckName);

	std::string GetDeckNameByRoomIndex(int index);

	// Boat information
	std::string mModelName;

	// Structure
	std::vector<Deck*> mpDecks;

	// Bounding volumes
	std::vector<AABB> mBoundingAABB;
	std::vector<DirectX::XMMATRIX> mFloorMatrix;
	std::vector<DirectX::XMMATRIX> mInverseFloorMatrix;

	EventLog mEventLog;
};
