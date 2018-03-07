#include "Test_Logic.h"

bool Test_Logic::MainTest()
{
	std::cout << "-------------------- Testing Boat and Log --------------------";
	std::cout << std::endl << std::endl;

	std::cout
		<< "Steps in this test:"
		<< "\n\t1. Create a Boat from a .boat file (which is holding a log)."
		<< "\n\t2. Fill boat with events and actions."
		<< "\n\t3. Clear some of the events and actions."
		<< "\n\t4. Save all stored information to files."
		<< "\n\t5. Delete the Boat and create a new one from the same .boat file."
		<< "\n\t6. Load the old log to recreate the scenario."
		<< "\n\t7. Save all stored information again, to new files."
		<< "\n\t8. Compare all the old files with the new files.";
	std::cout << "\nPress enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



	// Necessary variables
	Boat *myBoat = nullptr;
	Room *myRoom = nullptr;
	std::vector<int*> myActiveIndices;
	


	/**
	*	Create boat and load from file
	*/

	std::cout << "Loading boat from file...";
	myBoat = new Boat;
	myBoat->LoadFromFile_Boat("../../Savefiles/Boats/Vulcanus.boat");
	std::cout << "done!" << std::endl;

	std::cout << "Press enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



	/**
	*	Fill boat with event and actions
	*/

	Test_Logic::FillLog(myBoat, myActiveIndices);

	std::cout << "Press enter to continue..." << std::endl << std::endl;
	getchar();



	/**
	*	Clear some events and actions
	*/

	Test_Logic::ClearSome(myBoat, myActiveIndices);

	std::cout << "\nPress enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



	/**
	*	Save the log to a file
	*/

	std::cout
		<< "\nSaving log to files:"
		<< "\n  * Log: Savefiles/Testing/Logs/Log_first.log"
		<< "\n  * Meta: Savefiles/Testing/Metafiles/Log_metas/Log_first.meta"
		<< "\n  * Roomlog: Savefiles/Testring/Metafiles/Roomlog_metas_first/*";
	myBoat->SaveToFile_Log(
		"../../Savefiles/Testing/Logs/Log_first.log",
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_first.meta",
		"../../Savefiles/Testing/Metafiles/Roomlog_metas_first/");
	std::cout << "\nPress enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



	/**
	*	Scrap current log and load old log from file
	*/

	delete myBoat;
	myBoat = new Boat;
	myBoat->LoadFromFile_Boat("../../Savefiles/Boats/Vulcanus.boat");

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
	std::cout << "\nPress enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



	/**
	*	Save log to new file
	*/

	std::cout
		<< "\nSaving log to files:"
		<< "\n  * Log: Savefiles/Testing/Logs/Log_second.log"
		<< "\n  * Meta: Savefiles/Testing/Metafiles/Log_metas/Log_second.meta"
		<< "\n  * Roomlog: Savefiles/Testring/Metafiles/Roomlog_metas_second/*"
		<< "\n...";
	myBoat->SaveToFile_Log(
		"../../Savefiles/Testing/Logs/Log_second.log",
		"../../Savefiles/Testing/Metafiles/Log_metas/Log_second.meta",
		"../../Savefiles/Testing/Metafiles/Roomlog_metas_second/");
	std::cout << "Press enter to continue..." << std::endl << std::endl << std::endl;
	getchar();



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
	{
		return false;
	}
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

	std::cout << "\nPress enter to continue..." << std::endl << std::endl << std::endl;
	getchar();

	std::cout << "\n\nTest finished." << std::endl;
	std::cout << "--------------------------------------------------------------";
	std::cout << std::endl;
		
	getchar();



	/**
	*	Free memory
	*/
	
	for (int i = 0; i < (int)myActiveIndices.size(); i++)
		delete myActiveIndices[i];
	delete myBoat;

	return true;
}

void Test_Logic::FillLog(Boat *pBoat, std::vector<int*> &indices)
{
	indices.clear();

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
		
		// Waste some time to get some time differences between the events
		//for (int j = 0; j < 150000000; j++);
	}

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
		indices.push_back(new int(i));

		// Create and fill a description for the new action
		LogAction::Desc desc;
		desc.active = true;
		desc.pActionIndex;
		desc.pos_x = (float)i + 0.5f;
		desc.pos_z = (float)-i + 0.5f;
		desc.rotation = (i % 5) * 16;
		desc.roomName = myRoom->GetName();
		desc.start = true;
		desc.type = LogAction::GetTypeFromNumber(i % 9);

		// Add action to room
		myRoom->AddAction(desc);

		std::cout
			<< "  * Added '"
			<< LogAction::GetStringFromType(desc.type)
			<< "' to " << myRoom->GetName()
			<< " " << i + 1 << "/19" << std::endl;

		// Waste some time to get some time differences between the actions
		//for (int j = 0; j < 150000000; j++);
	}
}

void Test_Logic::ClearSome(Boat * pBoat, std::vector<int*>& indices)
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
			<< " (" << i << "/"
			<< std::floor(pBoat->GetRoomCount() / 2.f)
			<< ")";

		pRoom->ClearEvent(type_event);
	}



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
			<< " (" << i << "/"
			<< std::floor(pBoat->GetRoomCount() / 2.f)
			<< ")";

		// Use vector filled with simulated graphical actions
		// by the function 'FillLog'
		pRoom->ClearAction(indices[i]);
	}
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
