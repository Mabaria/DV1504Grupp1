#pragma once

#include "../BIS/BIS/System/System.h"
#include <stdlib.h>



class Simulator : public Observer<std::string>
{
public:
	/* Constructor
		Params:
			-sys (System)
				Needed so the simulator can call it from outside

			- chance (float) 
				Determines how much of a chance for an event to happen [0.0, 1.0]

			- seconds_between_a_new_chance (int)
				How much seconds before a new event can happen (based on chance)
	*/	
	Simulator(System& sys, float chance, int seconds_between_a_new_chance);
	~Simulator();

	void Update();
	virtual void Update(std::string * attribute);

	Room *pGetRandomRoom();

private:
	Timer mTimer;
	System* pSys;

	int mSecondsBetweenUpdates;
	int mMaxNumber;
	float mMaxLimit;
	Event::Type *mEvents;

	int mNrOfEvents;

	bool mRunSimulator;
};
