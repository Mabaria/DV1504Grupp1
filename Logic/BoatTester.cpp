#include "BoatTester.h"

void BoatTester::WriteTest()
{
	Boat *pBoat;

	/**
	*	Create boat
	*/
	pBoat = CreateBoat();

	/**
	*	Write boat to file
	*/
	WriteFile(pBoat, "../../SaveFiles/WriteTest.boat");

	delete pBoat;
}

void BoatTester::TestBoat()
{
	Boat *pBoat;
	Room *pRoom;

	std::string prefix = "  * ";

	/**
	*	Create boat
	*/
	pBoat = CreateBoat();



	/**
	*	Create auto event
	*/
	PrintHeader("Creating events to sensors");

	std::cout << "Adding Fire to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Maskinrum");
	pRoom->AddSensorEvent(Event::Type::Fire);

	// Sensor is set not to register water
	std::cout << "Adding Water to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Maskinrum");
	pRoom->AddSensorEvent(Event::Type::Water);

	std::cout << "Adding Gas to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Maskinrum");
	pRoom->AddSensorEvent(Event::Type::Gas);



	/**
	*	Check auto event
	*/
	std::vector<LogEvent*> events;

	std::cout << "\nChecking events in 'Maskinrum' in 'Huvuddäck':" << std::endl;
	std::cout << "(Should be Fire and Gas only)" << std::endl;
	pRoom = pBoat->GetRoomPointer("Maskinrum");
	events = pRoom->GetActiveEvents();

	std::cout << prefix << "Fire...";
	if (events[0]->GetType() != Event::Fire)
		throw ("Error expected Fire");
	std::cout << "ok!" << std::endl;

	std::cout << prefix << "Gas...";
	if (events[1]->GetType() != Event::Gas)
		throw ("Error expected Gas");
	std::cout << "ok!" << std::endl;

	if (events.size() != 2) // Should not register Event::Water
		throw ("Error: sensor detecting types it shouldn't");
	if (!(events[0]->GetType() == Event::Fire && events[1]->GetType() == Event::Gas))
		throw ("Error wrong auto events");



	/**
	*	Create plot event
	*/
	PrintHeader("Creating events from plotter");

	std::cout << "Adding Fire to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	pRoom->AddPlotterEvent(Event::Type::Fire);

	std::cout << "Adding Injury to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	pRoom->AddPlotterEvent(Event::Type::Injury);
	
	std::cout << "Adding Water to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	pRoom->AddPlotterEvent(Event::Type::Water);

	std::cout << "Adding Gas to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	pRoom->AddPlotterEvent(Event::Type::Gas);



	/**
	*	Check plot event
	*/
	std::cout << "\nChecking events in 'Skyddsrum' in 'Trossdäck':" << std::endl;
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	events = pRoom->GetActiveEvents();

	if (events.size() != 4)
		throw ("Error unexpected number of plot events handled");

	std::string type;
	for (int i = 0; i < 3; i++)
	{
		type = Event::GetString(Event::GetType(i));
		std::cout << prefix << "" << type << "...";

		if (events[i]->GetType() != Event::GetType(i))
			throw ("Error expected " + type);

		std::cout << "ok!" << std::endl;
	}



	/**
	*	Write boat to file
	*/
	WriteFile(pBoat, "../../SaveFiles/Testboat1.boat");
	std::cout << "Writing boat to 'SaveFiles/Testboat1.boat'." << std::endl;



	/**
	*	Read boat from file
	*/
	delete pBoat;
	pBoat = new Boat;
	if (!ReadFile(pBoat, "../../SaveFiles/Testboat1.boat"))
	{
		throw ("Error reading file");
	}
	std::cout << "Reading in boatfile from 'SaveFiles/Testboat1.boat'." << std::endl;



	/**
	*	Write boat to a new file
	*/
	WriteFile(pBoat, "../../SaveFiles/Testboat2.boat");
	std::cout << "Writing boat to 'SaveFiles/Testboat2.boat'." << std::endl;



	/**
	*	Compare the two files
	*/
	if (!CompareFiles("../../SaveFiles/Testboat1.boat", "../../SaveFiles/Testboat2.boat"))
		throw ("Error missmatching files from Write and Read");

	/**
	*	Add four events
	*/
	pRoom = pBoat->GetRoomPointer("Skyddsrum");
	pRoom->AddSensorEvent(Event::Type::Fire);
	pRoom->AddSensorEvent(Event::Type::Injury); // Sensor can't detect
	pRoom->AddSensorEvent(Event::Type::Water);	// Sensor can't detect
	pRoom->AddSensorEvent(Event::Type::Gas);

	/**
	*	Clear event
	*/
	pRoom->ClearEvent(Event::Type::Fire);
	pRoom->ClearEvent(Event::Type::Gas);
	pRoom->ClearEvent(Event::Type::Injury);
	pRoom->ClearEvent(Event::Type::Water);
	


	/**
	*	Check cleared events
	*/
	events = pRoom->GetActiveEvents();

	if (events.size() != 0)
		throw ("Error unexpected amount of events in room after clearing");



	/**
	*	Check eventlog
	*/
	int eventCount;			// Should be 2 (4 events -2 that wasn't detected by sensor)

	eventCount = pBoat->GetEventCount();

	if (eventCount != 2)
		throw ("Error unexpected amount of events in total");


	PrintHeader("Testing completed!");
	delete pBoat;
}

Boat* BoatTester::CreateBoat()
{
	std::string prefix = "  * ";

	PrintHeader("Building boat 'Testskepp'");

	Boat *newBoat = new Boat;
	newBoat->SetModelName("Testskepp");

	int inputs = 9;

	std::cout << "Creating deck 'Bryggdäck'...";
	newBoat->AddDeck("Bryggdäck");
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck:" << std::endl;
	std::cout << prefix << "slC...";
	newBoat->AddRoom("slC", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "SkyC...";
	newBoat->AddRoom("SkyC", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Brygga...";
	newBoat->AddRoom("Brygga", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "\nCreating deck 'Huvuddäck'...";
	newBoat->AddDeck("Huvuddäck");
	std::cout << "done!" << std::endl;
	std::cout << "Adding rooms to deck:" << std::endl;
	std::cout << prefix << "Skyddäck...";
	newBoat->AddRoom("Skyddäck", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Gång3...";
	newBoat->AddRoom("Gång3", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Tambur...";
	newBoat->AddRoom("Tambur", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Omformarrum...";
	newBoat->AddRoom("Omformarrum", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "CBRN...";
	newBoat->AddRoom("CBRN", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "\nCreating deck 'Trossdäck'...";
	newBoat->AddDeck("Trossdäck");
	std::cout << "done!" << std::endl;
	std::cout << "Adding rooms to deck:" << std::endl;
	std::cout << prefix << "Ammdurk...";
	newBoat->AddRoom("Ammdurk", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Lastrum...";
	newBoat->AddRoom("Lastrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "MC...";
	newBoat->AddRoom("MC", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Gång1...";
	newBoat->AddRoom("Gång1", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Apparatrum...";
	newBoat->AddRoom("Apparatrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Gång2...";
	newBoat->AddRoom("Gång2", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Hjälpmaskinrum...";
	newBoat->AddRoom("Hjälpmaskinrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Byssa...";
	newBoat->AddRoom("Byssa", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "SB Mäss...";
	newBoat->AddRoom("SB Mäss", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << prefix << "Skyddsrum...";
	newBoat->AddRoom("Skyddsrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	/**
	*	Send corresponding pointers
	*/
	std::cout << std::endl;

	std::cout << "\nAdding pointer from EventLog to Boat...";
	std::cout << "done!" << std::endl;

	return newBoat;
}

bool BoatTester::ReadFile(Boat *pBoat, std::string filePath)
{
	PrintHeader("Read boat from file");

	if (!pBoat->LoadFromFile_Boat(filePath))
		return false;

	return true;
}

void BoatTester::WriteFile(Boat *pBoat, std::string filePath)
{
	PrintHeader("Write boat to file");

	pBoat->SaveToFile_Boat(filePath);
}

bool BoatTester::CompareFiles(std::string filePath1, std::string filePath2)
{
	PrintHeader("Comparing files");

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
				if (line1 != line2)
				{
					std::cout << "failed. (mismatching lines)" << std::endl;
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

	std::cout << "ok!" << std::endl;
	return true;
}

void BoatTester::PrintHeader(std::string title)
{
	std::cout << std::endl;

	int size = (int)title.size();
	int width = 70;
	int padding = (width - size)/2;

	bool even = size % 2 == 0;

	if (size < 50)
	{
		std::cout << "|";
		for (int i = 0; i < width; i++)
			std::cout << "-";
		std::cout << "|\n|";
		for (int i = 0; i < padding; i++)
			std::cout << " ";

		std::cout << title;
		
		for (int i = 0; i < padding; i++)
			std::cout << " ";
		if (!even)
			std::cout << " ";
		std::cout << "|\n|";
		for (int i = 0; i < width; i++)
			std::cout << "-";
		std::cout << "|" << std::endl;
	}
	else
	{
		padding = 3;
		std::cout << "|";
		for (int i = 0; i < size+padding*2; i++)
			std::cout << "-";
		std::cout << "|\n|";
		for (int i = 0; i < padding; i++)
			std::cout << " ";
		std::cout << title;
		for (int i = 0; i < padding; i++)
			std::cout << " ";
		std::cout << "|\n|";
		for (int i = 0; i < size+padding*2; i++)
			std::cout << "-";
		std::cout << "|" << std::endl;
	}
	
	std::cout << std::endl;
	std::cout << std::endl;
}
