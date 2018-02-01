#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "Boat/Boat.h"
#include "Eventlog/EventLog.h"
#include "Activelog/ActiveLog.h"

namespace BoatTester
{
	void TestBoat();
	
	Boat* CreateBoat(EventLog *pEventLog);
	EventLog* CreateEventLog(ActiveLog *pActiveLog);
	
	bool ReadFile(Boat *boat, std::string filePath);
	void WriteFile(Boat *boat, std::string filePath);
	bool CompareFiles(std::string filePath1, std::string filePath2);
	
	void printHeader(std::string title);
}
