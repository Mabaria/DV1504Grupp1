#include "Test_Logic.h"

bool Test_Logic::MainTest(bool waitForInput, bool timeDelay)
{
	std::cout << "-------------------- Testing Boat and Log --------------------";
	std::cout << std::endl << std::endl;

	std::cout
		<< "Steps in this test:"
		<< "\n\t1. Create a Boat from a .boat file (which is holding a log)."
		<< "\n\t2. Fill boat with events and actions."
		<< "\n\t3. Check log and rooms for correct amount of events and "
		<< "actions."
		<< "\n\t4. Clear some of the events and actions."
		<< "\n\t5. Check log and rooms for correct amount of events and "
		<< "actions."
		<< "\n\t6. Delete the Boat and create a new one from the same .boat file."
		<< "\n\t7. Load the old log to recreate the scenario."
		<< "\n\t8. Check log and rooms for correct amount of events and "
		<< "actions."
		<< "\n\t9. Save all stored information again, to new files."
		<< "\n\t10. Compare all the old files with the new files."
		<< std::endl;

	if (waitForInput)
	{
		std::cout << "Press enter to continue...";
		getchar();
	}

	// Necessary variables
	Boat *myBoat = nullptr;
	Room *myRoom = nullptr;
	std::vector<int*> myActiveIndices;

	int totEventCount = 0;
	int totActionCount = 0;
	int activeEventCount = 0;
	int activeActionCount = 0;

	std::vector<int> roomEventCount;
	std::vector<int> roomActionCount;



	/**
	*	Create boat and load from file
	*/

	std::cout << "\nLoading boat from file...";
	myBoat = new Boat;
	myBoat->LoadFromFile_Boat("../../Savefiles/Boats/Vulcanus.boat");
	myBoat->SetLogPath("../../Savefiles/Testing/Logs/Log_first.log");
	myBoat->SetLogMetaPath("../../Savefiles/Testing/Metafiles/Log_metas/Log_first.meta");
	myBoat->SetRoomMetaDir("../../Savefiles/Testing/Metafiles/Roomlog_metas_first/");
	std::cout << "done!";

	// Clear old files
	myBoat->ClearFiles();

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl << std::endl;

	for (int i = 0; i < myBoat->GetRoomCount(); i++)
	{
		roomEventCount.push_back(0);
		roomActionCount.push_back(0);
	}

	/**
	*	Fill boat with event and actions
	*/

	Test_Logic::FillLog(
		timeDelay,
		myBoat,
		myActiveIndices,
		totEventCount,
		totActionCount,
		activeEventCount,
		activeActionCount,
		roomEventCount,
		roomActionCount);

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl;

	/**
	*	Check log and rooms for correct amount of events and actions.
	*/

	if (!Test_Logic::CheckAmount(
		myBoat,
		totEventCount,
		totActionCount,
		activeEventCount,
		activeActionCount,
		roomEventCount,
		roomActionCount))
		return false;

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl;

	/**
	*	Clear some events and actions
	*/

	Test_Logic::ClearSome(
		timeDelay,
		myBoat,
		myActiveIndices,
		totEventCount,
		totActionCount,
		activeEventCount,
		activeActionCount,
		roomEventCount,
		roomActionCount);

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl << std::endl;

	/**
	*	Check log and rooms for correct amount of events and actions.
	*/

	if (!Test_Logic::CheckAmount(
		myBoat,
		totEventCount,
		totActionCount,
		activeEventCount,
		activeActionCount,
		roomEventCount,
		roomActionCount))
		return false;

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl << std::endl;

	/**
	*	Scrap current log and load old log from file
	*/
	
	std::cout << "Deleting old boat and create a new one..";
	delete myBoat;
	myBoat = new Boat;
	myBoat->LoadFromFile_Boat("../../Savefiles/Boats/Vulcanus.boat");
	myBoat->SetLogPath("../../Savefiles/Testing/Logs/Log_second.log");
	myBoat->SetLogMetaPath("../../Savefiles/Testing/Metafiles/Log_metas/Log_second.meta");
	myBoat->SetRoomMetaDir("../../Savefiles/Testing/Metafiles/Roomlog_metas_second/");
	std::cout << "done!";

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl;

	// Clear old files
	myBoat->ClearFiles();

	// Read log from file
	std::cout
		<< "\nLoading log from files:"
		<< "\n  * Log: Savefiles/Testing/Logs/Log_first.log"
		<< "\n  * Meta: Savefiles/Testing/Metafiles/Log_metas/Log_first.meta"
		<< "\n  * Roomlog: Savefiles/Testring/Metafiles/Roomlog_metas_first/*";
	if (!myBoat->LoadFromFile_Log(
		"../../Savefiles/Testing/Logs/Log_first.log",
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_first.meta",
		"../../Savefiles/Testing/Metafiles/Roomlog_metas_first/"))
		return false;
	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl << std::endl;

	/**
	*	Check log and rooms for correct amount of events and actions.
	*/

	if (!Test_Logic::CheckAmount(
		myBoat,
		totEventCount,
		totActionCount,
		activeEventCount,
		activeActionCount,
		roomEventCount,
		roomActionCount))
		return false;

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl;

	/**
	*	Save log to new file
	*/

	std::cout
		<< "\nSaving log to files:"
		<< "\n  * Log: Savefiles/Testing/Logs/Log_second.log"
		<< "\n  * Meta: Savefiles/Testing/Metafiles/Log_metas/Log_second.meta"
		<< "\n  * Roomlog: Savefiles/Testring/Metafiles/Roomlog_metas_second/*";
	myBoat->SaveToFile_Log(
		"../../Savefiles/Testing/Logs/Log_second.log",
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_second.meta",
		"../../Savefiles/Testing/Metafiles/Roomlog_metas_second/");
	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << std::endl;

	/**
	*	Compare old and new saved files
	*/

	// Compare .log files
	std::cout
		<< "\nComparing files"
		<< "\n  * Log files...";

	if (!Test_Logic::CompareFiles(
		"../../Savefiles/Testing/Logs/Log_first.log",
		"../../Savefiles/Testing/Logs/Log_second.log"))
	{
		return false;
	}
	std::cout << "ok!";



	// Compare .meta (log) files
	std::cout
		<< "\n  * Meta (log) files...";

	if (!Test_Logic::CompareFiles(
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_first.meta",
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_second.meta"))
		return false;
	std::cout << "ok!";



	// Compare .meta (roomlog) files
	std::cout
		<< "\n  * Meta (roomlog) files...";

	for (int i = 0; i < myBoat->GetRoomCount(); i++)
	{
		myRoom = myBoat->GetRoomPointerAt(i);

		// Check each roomlog meta file
		if (!Test_Logic::CompareFiles(
			"../../Savefiles/Testing/Metafiles/Roomlog_metas_first/"
			+ myRoom->GetName()
			+ ".meta",
			"../../Savefiles/Testing/Metafiles/Roomlog_metas_second/"
			+ myRoom->GetName()
			+ ".meta"))
		{
			return false;
		}
	}
	std::cout << "ok!";

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << "\n\nTest finished." << std::endl;

	if (waitForInput)
	{
		std::cout << "\nPress enter to continue...";
		getchar();
	}

	std::cout << "--------------------------------------------------------------";
	std::cout << std::endl;



	/**
	*	Free memory
	*/

	for (int i = 0; i < (int)myActiveIndices.size(); i++)
		delete myActiveIndices[i];
	delete myBoat;

	return true;
}

void Test_Logic::FillLog(
	bool timeDelay,
	Boat *pBoat,
	std::vector<int*> &indexOutput,
	int &totEventOutput,
	int &totActionOutput,
	int &activeEventOutput,
	int &activeActionOutput,
	std::vector<int> &roomEventCountOutput,
	std::vector<int> &roomActionCountOutput)
{
	for (int i = 0; i < (int)indexOutput.size(); i++)
		delete indexOutput[i];		
	indexOutput.clear();

	Room* myRoom;
	Event::Type type_event;

	/**
	*	Fill boat with events.
	*		Each room will get an event.
	*		Type will vary.
	*/

	std::cout << "Filling rooms with events:" << std::endl;

	for (int i = 0; i < pBoat->GetRoomCount(); i++)
	{
		myRoom = pBoat->GetRoomPointerAt(i);
		type_event = Event::GetType(i % 4);

		// Add event to room
		// Plotter event won't check sensor inputs.
		myRoom->AddPlotterEvent(type_event);

		std::cout
			<< "  * Added '"
			<< Event::GetString(type_event)
			<< "' to " << myRoom->GetName()
			<< " " << i+1 << "/19" << std::endl;

		totEventOutput++;
		activeEventOutput++;
		roomEventCountOutput[i]++;
		
		// Waste some time to get some time differences between the events
		if (timeDelay)
			for (int j = 0; j < 100000000; j++);
	}

	if (timeDelay)
		for (int j = 0; j < 100000000; j++);

	std::cout << std::endl;

	/**
	*	Fill boat with actions.
	*		Each room will get an action.
	*		Action indices (used in graphical actions) will be created on the fly.
	*		Type, Position and Rotation will vary.
	*/

	std::cout << "Filling rooms with actions:" << std::endl;

	for (int i = 0; i < pBoat->GetRoomCount(); i++)
	{
		myRoom = pBoat->GetRoomPointerAt(i);

		// Simulate new graphical actions
		indexOutput.push_back(new int(i));

		// Create and fill a description for the new action
		LogAction::Desc desc;
		desc.active = true;
		desc.pActionIndex = indexOutput[i];
		desc.pos_x = (float)i + 0.5f;
		desc.pos_z = (float)-i + 0.5f;
		desc.rotation = (i % 5) * 16;
		desc.roomName = myRoom->GetName();
		desc.start = true;
		desc.type = LogAction::GetTypeFromNumber(i % 9);
		desc.numberOnIcon = -1;

		// Add action to room
		myRoom->AddAction(desc);

		totActionOutput++;
		activeActionOutput++;
		roomActionCountOutput[i]++;

		std::cout
			<< "  * Added '"
			<< LogAction::GetStringFromType(desc.type)
			<< "' to " << myRoom->GetName()
			<< " " << i + 1 << "/19" << std::endl;

		if (timeDelay)
			for (int j = 0; j < 100000000; j++);
	}

	if (timeDelay)
		for (int j = 0; j < 100000000; j++);
}

void Test_Logic::ClearSome(
	bool timeDelay,
	Boat *pBoat,
	std::vector<int*> &indexOutput,
	int &totEventOutput,
	int &totActionOutput,
	int &activeEventOutput,
	int &activeActionOutput,
	std::vector<int> &roomEventCountOutput,
	std::vector<int> &roomActionCountOutput)
{
	std::cout << "\nClearing events";
	Room *pRoom = nullptr;
	Event::Type type_event;
	LogAction::Type type_action;

	for (int i = 0; i < std::floor(pBoat->GetRoomCount() / 2.f); i++)
	{
		pRoom = pBoat->GetRoomPointerAt(i * 2);
		type_event = Event::GetType((i * 2) % 4);

		std::cout
			<< "\n  * "
			<< pRoom->GetName()
			<< " - "
			<< Event::GetString(type_event)
			<< " (" << i + 1 << "/"
			<< std::floor(pBoat->GetRoomCount() / 2.f)
			<< ")";

		pRoom->ClearEvent(type_event);

		// The log creates a new (inactive) event when clearing
		totEventOutput++;
		activeEventOutput--;
		roomEventCountOutput[i * 2]--;

		if (timeDelay)
			for (int j = 0; j < 100000000; j++);
	}

	if (timeDelay)
		for (int j = 0; j < 100000000; j++);

	std::cout << "\n\nClearing actions";

	for (int i = 0; i < std::floor(pBoat->GetRoomCount() / 2.f); i++)
	{
		pRoom = pBoat->GetRoomPointerAt(i * 2);
		type_action = LogAction::GetTypeFromNumber(i % 9);

		std::cout
			<< "\n  * "
			<< pRoom->GetName()
			<< " - "
			<< LogAction::GetStringFromType(type_action)
			<< " (" << i + 1  << "/"
			<< std::floor(pBoat->GetRoomCount() / 2.f)
			<< ")";

		// Use vector filled with simulated graphical actions
		// by the function 'FillLog'
		pRoom->ClearAction(indexOutput[i * 2]);

		// The log creates a new (inactive) action when clearing
		totActionOutput++;
		activeActionOutput--;
		roomActionCountOutput[i * 2]--;

		if (timeDelay)
			for (int j = 0; j < 100000000; j++);
	}

	if (timeDelay)
		for (int j = 0; j < 100000000; j++);
}

bool Test_Logic::CheckAmount(
	Boat * pBoat,
	int & totEventOutput,
	int & totActionOutput,
	int & activeEventOutput,
	int & activeActionOutput,
	std::vector<int>& roomEventCountOutput,
	std::vector<int>& roomActionCountOutput)
{
	Room *pRoom = nullptr;

	std::cout << "Check amount of events and actions:";
	std::cout << "\n  * Log...";
	if (totEventOutput != pBoat->GetTotalEventCount() ||
		totActionOutput != pBoat->GetTotalActionCount())
		return false;
	if (activeEventOutput != pBoat->GetActiveEventCount() ||
		activeActionOutput != pBoat->GetActiveActionCount())
		return false;
	std::cout << "ok!";

	std::cout << "\n  * Rooms...";
	for (int i = 0; i < (int)pBoat->GetRoomCount(); i++)
	{
		pRoom = pBoat->GetRoomPointerAt(i);

		if (pRoom->GetActiveEventCount() != roomEventCountOutput[i] ||
			pRoom->GetActionCount() != roomActionCountOutput[i])
			return false;
	}
	std::cout << "ok!";

	return true;
}

bool Test_Logic::CompareFiles(std::string filePath1, std::string filePath2)
{
	std::string line1, line2;
	bool fileFlag1, fileFlag2;
	std::ifstream file1 (filePath1);
	std::ifstream file2 (filePath2);

	fileFlag1 = true;
	fileFlag2 = true;

	if (file1.is_open() && file2.is_open())
	{

		while (fileFlag1 && fileFlag2)
		{
			if (!getline(file1, line1))
				fileFlag1 = false;
			if (!getline(file2, line2))
				fileFlag2 = false;

			if (fileFlag1 && fileFlag2)
			{
				if (line1.size() != line2.size())
					return false;
				
				for (int i = 0; i < (int)line1.size(); i++)
				{
					if (line1[i] != line2[i]) // Compare char-wise
					{
						std::cout
							<< "failed. (different characters)"
							<< std::endl;
						return false;
					}
				}
			}
		}

		if (fileFlag1 != fileFlag2) /* Both should be false, else one file is
									   bigger than the other */
		{
			std::cout << "failed. (different amount of lines)" << std::endl;
			return false;
		}
	}
	else // File(s) can't be opened
		return false;

	return true;
	
}
