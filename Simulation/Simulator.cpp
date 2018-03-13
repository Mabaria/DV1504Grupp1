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

	// --- To add more events change these variables
	this->mNrOfEvents = 3;
	this->mEvents 
		= new Event::Type[this->mNrOfEvents]
	{ 
		Event::Water, 
		Event::Fire,
		Event::Gas,
	};
	// ---

	this->pSys = &sys;
	this->mTimer.StartTimer();
	srand((unsigned int)time(NULL));

	this->mRunSimulator = false;
	this->pSys->AddObserver(this);
}

Simulator::~Simulator()
{
	delete this->mEvents;
}

void Simulator::Update()
{
	if (this->mRunSimulator)
	{
		Room *pSelected = this->pGetRandomRoom();
		if (pSelected)
		{
			this->pSys->UpdateRoom(pSelected);
		}
	}
}

void Simulator::Update(std::string * attribute)
{
	if (attribute->compare("simulator_(de)activate") == 0)
	{
		this->mRunSimulator = !this->mRunSimulator;
		this->mTimer.StartTimer();
	}
}

Room * Simulator::pGetRandomRoom()
{
	// Make sure an event is added
	bool event_added = false;

	Room * pSelected = nullptr; // Selected Room

	// if infinite loop was discovered and exit can be made
	int fail_safe_count = 0;

	// If its time to do a roll
	if (mSecondsBetweenUpdates <= this->mTimer.GetSeconds())
	{
		this->mTimer.StartTimer();
		float number = (float)(rand() % this->mMaxNumber); // Do a roll

		if (number <= this->mMaxLimit) // Add event if roll succeeded
		{
			
			int rEvent	= 0;		// Random Event
			int rRoom	= 0;		// Random Room

			// Infinite loop when all rooms has events added
			// Try to add event till it works (not optimised but works)
			do {
				if (!event_added)
				{
					rEvent = rand() % this->mNrOfEvents;
				}

				do {

					if (!pSelected || !event_added)
					{
						rRoom = rand() % this->pSys->GetNrOfRooms();
					}
					pSelected = this->pSys->GetRoomByIndex(rRoom);

				} while (!pSelected);


				event_added = pSelected->AddSensorEvent(this->mEvents[rEvent]);


				fail_safe_count++;
			} while (!event_added && fail_safe_count <= 50);
		}

		/* -- For debugging --
		if (!event_added)
			std::cout << "Failed to place event!" << std::endl;*/
	}

	return pSelected;
}
