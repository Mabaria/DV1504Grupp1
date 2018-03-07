#include "Simulator.h"
#include <iostream>

Simulator::Simulator(System& sys)
{
	this->pSys = &sys;
	this->mSecondsBetweenUpdates = 1;
	srand((unsigned int)time(NULL));
	timer.StartTimer();
}

Simulator::~Simulator()
{
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
	Room * pSelected = nullptr;

	if (mSecondsBetweenUpdates <= timer.GetSeconds())
	{
		float number = (float)(rand() % 500); // Get a number
		float x = 500.0f;

		Event::Type types[] = { Event::Fire, Event::Water, Event::Gas };
		int nr_of_types = (int)floor(sizeof(types) / sizeof(Event::Type));

		if (number <= x) // x / number chance of getting an event
		{
			int type = rand() % nr_of_types;
			// Make sure an event is added
			do {

				int random_number = rand() % this->pSys->GetNrOfRooms();
				pSelected = this->pSys->GetRoomByIndex(random_number);
				pSelected->AddSensorEvent(types[type]);

			} while (pSelected->GetActiveEventIndex() == -1);
		}

		timer.StartTimer();
	}

	return pSelected;
}
