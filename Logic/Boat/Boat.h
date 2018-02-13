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

#include "../../IO/Picking.h"

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
	//void AddRoom(std::string roomName, std::string deckName);
	void AddRoom(std::string roomName,
		std::string deckName,
		std::vector<Event::Type> inputs);
	Room* GetRoomPointer(std::string roomName, std::string deckName);
	Room* GetPickedRoom(Ray ray);


	// Log specific
	void SetEventLog(EventLog *pEventLog);
	//void SetActiveEventIndex(std::string roomName, std::string deckName, int index);

	// Event specific
	void CreateAutoEvent(Event::Type type, std::string roomName, std::string deckName);
	void CreatePlotEvent(Event::Type type, std::string roomName, std::string deckName);
	void ClearEvent(Event::Type type, std::string roomName, std::string deckName);
	std::vector<Event::Type> GetEventsInRoom(std::string roomName, std::string deckName);

	// Disk specific
	void WriteFile(std::string filePath) const;
	bool ReadFile(std::string filePath);

	// Room data specific
	bool LoadBoundingBoxes(
		Mesh *meshList,
		DirectX::XMMATRIX **matrixList,
		int amount);
	//MeshObject *GetBoundingMeshObject(int index);

	// Boat mesh specific
	//x bool LoadBoatMesh();
	//MeshObject *GetDeckMeshObject(int index);

	// Bounding volumes
	//void StoreBoundings(std::vector<AABB> boundings);

private:
	
	// Returns -1 if item is not found
	int GetRoomIndex(std::string roomName, std::string deckName);
	int GetDeckIndex(std::string deckName);

	RoomDesc FillRoomDescFromLine(std::string line);
	DeckDesc FillDeckDescFromLine(std::string line);
	std::string GetNameFromLine(std::string line, char until = NULL);

	std::string GetDeckNameByRoomIndex(int index);

	std::string mModelName;

	std::vector<Deck*> mpDecks;
	std::vector<Room*> mpRooms;

	//std::vector<Mesh> mBoundingMesh;
	//std::vector<MeshObject> mBoundingMeshObjects;
	std::vector<AABB> mBoundingAABB;
	std::vector<DirectX::XMMATRIX> mFloorMatrix;
	std::vector<DirectX::XMMATRIX> mInverseFloorMatrix;
	//std::vector<Mesh> mDeckMesh;
	//std::vector<MeshObject> mDeckMeshObject;
	//! World matrices are inside the MeshObject class
	//std::vector<DirectX::XMMATRIX> mDeckWorldMatrix;

	EventLog *mpEventLog;

	//std::vector<AABB> mBoundings;
};
