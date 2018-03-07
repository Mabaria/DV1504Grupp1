#pragma once

#include "../BIS/BIS/System/System.h"
#include <stdlib.h>

class Simulator
{
public:
	Simulator(System& sys);
	~Simulator();

	void Update();

	Room *pGetRandomRoom();

private:
	Timer timer;
	System* pSys;
	int mSecondsBetweenUpdates;
};
