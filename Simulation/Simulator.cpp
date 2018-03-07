#include "Simulator.h"
#include <iostream>

Simulator::Simulator(System& sys, float chance, int seconds_between_a_new_chance)
{
	if (chance < 0.0f)
		chance = 0.0f;
	else if (chance > 1.0f)
		chance = 1.0f;

	this->mSecondsBetweenUpdates = seconds_between_a_new_chance;
	
	// Max number available to random
	this->mMaxNumber	= 100;	

	// Max limit for number to random to activate an event
	this->mMaxLimit		= this->mMaxNumber * chance;	

	this->mNrOfEvents = 3;
	this->mEvents 
		= new Event::Type[this->mNrOfEvents]
	{ 
		Event::Water, 
		Event::Fire, 
		Event::Gas, 
	};


	this->pSys = &sys;
	this->timer.StartTimer();
	srand((unsigned int)time(NULL));
}

Simulator::~Simulator()
{
	delete this->mEvents;
}

void Simulator::Update()
{
	Room *pSelected = this->pGetRandomRoom();
	if (pSelected)
	{
		this->pSys->UpdateRoom(pSelected);
	}
}

Room * Simulator::pGetRandomRoom()
{
	Room * pSelected = nullptr; // Selected Room

	if (mSecondsBetweenUpdates <= this->timer.GetSeconds())
	{
		this->timer.StartTimer();

		float number = (float)(rand() % this->mMaxNumber); // Do a roll

		if (number <= this->mMaxLimit) // Add event if roll succeeded
		{
			
			int rEvent	= 0;		// Random Event
			int rRoom	= 0;		// Random Room

			// Make sure an event is added
			bool event_added = false;
			do {
				if(!event_added)
					rEvent = rand() % this->mNrOfEvents;

				if(!pSelected)
					rRoom = rand() % this->pSys->GetNrOfRooms();

				pSelected = this->pSys->GetRoomByIndex(rRoom);
				event_added = pSelected->AddSensorEvent(this->mEvents[rEvent]);

			} while (pSelected->GetActiveEventIndex() == -1 || event_added);
		}
	}

	return pSelected;
}
