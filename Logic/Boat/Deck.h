#pragma once

#include <string>
#include <vector>
#include "Room.h"
#include "../Eventlog/EventLog.h"

/**
*	Deck is a simple structure that holds offset and length of its own rooms in
* the Boat room list.
*/

class Deck
{
public:

	struct Desc
	{
		int index;
		int roomOffset;
		std::string name;
		EventLog *pEventLog;
	};



	Deck();
	Deck(Deck::Desc desc);
	~Deck();
	
	// Deck specific
	void SetIndex(int index);
	void SetName(std::string name);
	void InitFromDesc(Deck::Desc desc);
	std::string GetName() const;

	// Room specific
	bool AddRoom(std::string name, int inputs);
	bool AddRoom(std::string line);
	void SetRoomOffset(int index);
	int GetRoomCount() const;
	int GetRoomOffset() const;
	int GetRoomIndex_Boat(std::string name) const;
	int GetRoomIndex_Deck(std::string name) const;
	Room* GetRoomPointer(std::string name) const;
	Room* GetRoomPointerAt(int index) const;

	// Disk specific
	std::string GetString() const;
	std::string GetRoomStringAt(int index) const;
	static Deck::Desc FillDeckDescFromLine(std::string line);
	void SaveRoomLogs(std::string folderPath) const;
	bool LoadRoomLogs(std::string folderPath);

private:

	// Deck specific
	int mIndex;
	std::string mName;

	// Room specific
	int mRoomOffset;
	std::vector<Room*> mpRooms;
	
	// Log specific
	EventLog *mpEventLog;
};

