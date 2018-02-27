#pragma once

#include <iostream>
#include "Eventlog/EventLog.h"
#include "Boat/Boat.h"

namespace Test_Log
{

	/**
	*	HOW TO USE
	*		Call MainTest(), it will test everything for you and return true
	*		if the test passes.
	*/

	// Log
	bool MainTest(); // To test all, call this function
	void FillLog(Boat *pBoat, EventLog *pLog);

	// Disk
	bool CompareFiles(std::string filePath1, std::string filePath2);
}
