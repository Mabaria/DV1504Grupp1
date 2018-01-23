#pragma once

#include <vector>
#include "Deck.h"

// TODO: ADD POINTERS!!!

class DeckHandler
{
public:

	DeckHandler();
	~DeckHandler();

	void AddDeck(std::string name);
	void AddRoom(std::string name, std::string deck);

private:

	std::vector<Deck> mDecks;

};
