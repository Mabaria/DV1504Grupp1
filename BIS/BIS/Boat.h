#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Deck.h"
#include "Room.h"
#include "EventLog.h"
#include "Event.h"

class Boat
{
public:

	Boat();
	~Boat();

	void CreateEvent(Event::Type type, std::string roomName, std::string deckName);
	
	void AddDeck(std::string name);
	void AddRoom(std::string roomName, std::string deckName);
	void AddRoom(std::string roomName, std::string deckName, int inputs[NR_OF_EVENT_TYPES]);

	std::string GetModelName() const;

	void SetModelName(std::string name);
	void SetEventLog(EventLog *pEventLog);
	void SetRoomEventIndex(std::string roomName, std::string deckName, int index);

	void WriteFile(std::string filePath);
	void ReadFile(std::string filePath);

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
