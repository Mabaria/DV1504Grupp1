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
	Deck* GetDeckPointerAt(int index);

	// Room specific
	//void AddRoom(std::string roomName, std::string deckName);
	void AddRoom(std::string roomName,
		std::string deckName,
		std::vector<Event::Type> inputs);
	Room* GetPickedRoom(Ray ray);
	Room* GetRoomPointerAt(int index);

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
	bool LoadBoundingBoxes();
	MeshObject *GetBoundingMeshObject(int index);

	// Boat mesh specific
	bool LoadBoatMesh();
	MeshObject *GetDeckMeshObject(int index);

	// Bounding volumes
	//void StoreBoundings(std::vector<AABB> boundings);

private:
	
	// Returns -1 if item is not found
	int GetRoomIndex(std::string roomName, std::string deckName);
	int GetDeckIndex(std::string deckName);

	std::string GetDeckNameByRoomIndex(int index);

	std::string mModelName;

	std::vector<Deck> mDecks;
	std::vector<Room> mRooms;

	std::vector<Mesh> mBoundingMesh;
	std::vector<MeshObject> mBoundingMeshObjects;
	std::vector<AABB> mBoundingAABB;
	std::vector<Mesh> mDeckMesh;
	std::vector<MeshObject> mDeckMeshObject;
	//! World matrices are inside the MeshObject class
	//std::vector<DirectX::XMMATRIX> mDeckWorldMatrix;

	EventLog *mpEventLog;

	//std::vector<AABB> mBoundings;
};
