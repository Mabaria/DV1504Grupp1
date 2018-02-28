#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "Boat/Boat.h"
#include "Eventlog/EventLog.h"

namespace BoatTester
{
	void WriteTest();
	void TestBoat();
	
	Boat* CreateBoat();
	
	bool ReadFile(Boat *boat, std::string filePath);
	void WriteFile(Boat *boat, std::string filePath);
	bool CompareFiles(std::string filePath1, std::string filePath2);
	
	void PrintHeader(std::string title);
}
