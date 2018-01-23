#include "DeckHandler.h"

DeckHandler::DeckHandler()
{
}

DeckHandler::~DeckHandler()
{
}

void DeckHandler::AddDeck(std::string name)
{
	this->mDecks.push_back(Deck(name));
}

void DeckHandler::AddRoom(std::string roomName, std::string deckName)
{
	bool found = false;

	for (int i = 0; i < this->mDecks.size() && !found; i++)
	{
		if (this->mDecks[i].GetName() == deckName)
		{
			found = true;
			this->mDecks[i].AddRoom(roomName);
		}
	}

	if (!found)
	{
		throw("Can't add room " + roomName + " to deck " + deckName);
	}
}
