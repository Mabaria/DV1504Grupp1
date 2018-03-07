#pragma once

#include "../BIS/BIS/System/System.h"
#include <stdlib.h>

class Simulator
{
public:
	Simulator(System& sys, float chance, int seconds_between_a_new_chance);
	~Simulator();

	void Update();

	Room *pGetRandomRoom();

private:
	Timer timer;
	System* pSys;

	int mSecondsBetweenUpdates;
	int mMaxNumber;
	float mMaxLimit;
	Event::Type* mEvents;

	int mNrOfEvents;
};
