#pragma once

#include <string>
#include "EventType.h"

class Room
{
public:

	Room();
	~Room();

	void AddEvent(EventType type);

	void SetName(std::string name);
	void SetRoomEventIndex(int index);

	std::string GetName() const;
	int GetRoomEventIndex() const;

private:

	std::string mName;
	int mRoomEventIndex;
};
