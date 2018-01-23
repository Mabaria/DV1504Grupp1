#pragma once

#include <vector>
#include "EventLog.h"



class Room
{
public:

	Room(std::string name, std::string deckName);
	~Room();

	const bool HasEvents() const;

	// Returns true if successful
	const bool AddEvent(EventType type);
	const bool RemoveEvent(EventType type);

	const std::string GetName() const;
	const std::string GetDeck() const;

private:

	std::string mName;
	std::string mDeckName;

	std::vector<> mEvents;
};
