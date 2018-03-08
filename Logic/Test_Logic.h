#pragma once

#include <iostream>
#include "Eventlog/EventLog.h"
#include "Boat/Boat.h"

namespace Test_Logic
{

	/**
	*	HOW TO USE
	*		Call MainTest(), it will test everything for you and return true
	*		if the test passes.
	*/

	// Log
	bool MainTest(bool waitForInput = true); // To test all, call this function

	void FillLog(
		Boat *pBoat,
		std::vector<int*> &indexOutput,
		int &totEventOutput,
		int &totActionOutput,
		int &activeEventOutput,
		int &activeActionOutput,
		std::vector<int> &roomEventCountOutput,
		std::vector<int> &roomActionCountOutput);

	void ClearSome(
		Boat *pBoat,
		std::vector<int*> &indexOutput,
		int &totEventOutput,
		int &totActionOutput,
		int &activeEventOutput,
		int &activeActionOutput,
		std::vector<int> &roomEventCountOutput,
		std::vector<int> &roomActionCountOutput);

	bool CheckAmount(
		Boat *pBoat,
		int &totEventOutput,
		int &totActionOutput,
		int &activeEventOutput,
		int &activeActionOutput,
		std::vector<int> &roomEventCountOutput,
		std::vector<int> &roomActionCountOutput);

	// Disk
	bool CompareFiles(std::string filePath1, std::string filePath2);
}
