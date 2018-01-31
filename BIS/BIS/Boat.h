#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Deck.h"
#include "Room.h"
#include "EventLog.h"
#include "Event.h"

// TODO: Delete GetRoomIndex that only finds the room by name
// (Two rooms can share the same name)

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

	// Room specific
	//void AddRoom(std::string roomName, std::string deckName);
	void AddRoom(std::string roomName, std::string deckName, int inputs[NR_OF_EVENT_TYPES]);

	// Log specific
	void SetEventLog(EventLog *pEventLog);
	//void SetActiveEventIndex(std::string roomName, std::string deckName, int index);

	// Event specific
	void CreateEvent(Event::Type type, std::string roomName, std::string deckName);

	// Disk specific
	void WriteFile(std::string filePath);
	void ReadFile(std::string filePath);

	// Bounding volumes
	//void StoreBoundings(std::vector<AABB> boundings);

private:
	
	// Returns -1 if item is not found
	int GetRoomIndex(std::string roomName);
	int GetRoomIndex(std::string roomName, std::string deckName);
	int GetDeckIndex(std::string deckName);

	std::string GetDeckNameByRoomIndex(int index);

	std::string mModelName;

	std::vector<Deck> mDecks;
	std::vector<Room> mRooms;

	//std::vector<AABB> mBoundings;
};
