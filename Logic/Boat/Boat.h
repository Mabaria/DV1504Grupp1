#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Deck.h"
#include "Room.h"
#include "../Eventlog/EventLog.h"
#include "../Event/Event.h"

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

	// Log specific
	void SetEventLog(EventLog *pEventLog);
	//void SetActiveEventIndex(std::string roomName, std::string deckName, int index);

	// Event specific
	void CreateAutoEvent(Event::Type type, std::string roomName, std::string deckName);
	void CreatePlotEvent(Event::Type type, std::string roomName, std::string deckName);
	void ClearEvent(Event::Type type, std::string roomName, std::string deckName);
	std::vector<Event::Type> GetEventsInRoom(std::string roomName, std::string deckName);

	// Disk specific
	void WriteFile(std::string filePath);
	bool ReadFile(std::string filePath);

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

	EventLog *mpEventLog;

	//std::vector<AABB> mBoundings;
};
