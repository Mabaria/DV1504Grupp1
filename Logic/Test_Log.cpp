#include "Test_Log.h"

bool Test_Log::MainTest()
{
	std::cout << "---------------------- Testing EventLog ----------------------";
	std::cout << std::endl << std::endl;
	
	Boat *myBoat = new Boat;
	EventLog *myLog = new EventLog;

	myBoat->SetEventLog(myLog);
	myBoat->ReadFile("../../Savefiles/data.boat");
	
	// Add an event to each room in boat
	Test_Log::FillLog(myBoat, myLog);

	// One event has been added to each room. The amount of events
	// should now be the same as the amount of rooms.
	if (myLog->GetEventCount() != myBoat->GetRoomCount())
		return false;

	// Save log to file
	std::cout << "\nSaving log to file 'Savefiles/testlog_first.log'...";
	myLog->SaveToFile("../../Savefiles/testlog_first.log");
	std::cout << "done!" << std::endl;
	
	// Scrap current log
	delete myLog;
	
	// Create a new log
	myLog = new EventLog;

	// Read log from file
	std::cout << "Loading log from file 'Savefiles/testlog_first.log'...";
	if (!myLog->LoadFromFile("../../Savefiles/testlog_first.log"))
		return false;
	std::cout << "done!" << std::endl;
	
	// Save log to a new file
	std::cout << "Saving log to new file 'Savefiles/testlog_second.log'...";
	myLog->SaveToFile("../../Savefiles/testlog_second.log");
	std::cout << "done!" << std::endl << std::endl;

	// Compare the two .log files
	if (!Test_Log::CompareFiles(
		"../../Savefiles/testlog_first.log",
		"../../Savefiles/testlog_second.log"))
	{
		return false;
	}

	std::cout << std::endl << "Test for Log completed. If this text is shown, "
		<< "the log passed the test with exelent results!!!" << std::endl;
	std::cout << "If you want to review the .log files, please check them "
		<< "under the folder 'Savefiles/'";
	
	std::cout << std::endl << std::endl;
	std::cout << "--------------------------------------------------------------";
	std::cout << std::endl;
		
	getchar();

	delete myLog;
	delete myBoat;

	return true;
}

// Log
void Test_Log::FillLog(Boat *pBoat, EventLog *pLog)
{
	Room* myRoom;
	Event::Type type;
	for (int i = 0; i < pBoat->GetRoomCount(); i++)
	{
		myRoom = pBoat->GetRoomPointerAt(i);
		type = (Event::Type) (i % 4);

		myRoom->AddPlotterEvent(type);

		std::cout << "Added " << Event::GetString(type)
			<< " to " << myRoom->GetName()
			<< " " << i+1 << "/19" << std::endl;
		
		// Waste some time to get some time differences between the events
		for (int j = 0; j < 200000000; j++);
	}
}

bool Test_Log::CompareFiles(std::string filePath1, std::string filePath2)
{
	std::cout << "Comparing '" << filePath1 << "' and '" << filePath2 << "'...";

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
						return false;
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

	std::cout << "ok!" << std::endl;
	return true;
	
}
