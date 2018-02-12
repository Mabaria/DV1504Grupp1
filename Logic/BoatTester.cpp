#include "BoatTester.h"

void BoatTester::WriteTest()
{
	EventLog *pEventLog;
	Boat *pBoat;

	pEventLog = new EventLog;

	/**
	*	Create boat
	*/
	pBoat = CreateBoat(pEventLog);

	/**
	*	Write boat to file
	*/
	WriteFile(pBoat, "../../SaveFiles/WriteTest.boat");

	delete pBoat;
	delete pEventLog;
}

void BoatTester::TestBoat()
{
	EventLog *pEventLog;
	Boat *pBoat;

	pEventLog = new EventLog;

	/**
	*	Create boat
	*/
	pBoat = CreateBoat(pEventLog);



	/**
	*	Create auto event
	*/
	PrintHeader("Creating events to sensors");

	std::cout << "Adding Fire to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pBoat->CreateAutoEvent(Event::Fire, "Maskinrum", "Huvuddäck");

	// Sensor is set not to register water
	std::cout << "Adding Fire to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pBoat->CreateAutoEvent(Event::Water, "Maskinrum", "Huvuddäck");

	std::cout << "Adding Fire to 'Maskinrum' in 'Huvuddäck'" << std::endl;
	pBoat->CreateAutoEvent(Event::Gas, "Maskinrum", "Huvuddäck");



	/**
	*	Check auto event
	*/
	std::vector<Event::Type> events;

	events = pBoat->GetEventsInRoom("Maskinrum", "Huvuddäck");
	std::cout << "\nChecking events in 'Maskinrum' in 'Huvuddäck':" << std::endl;
	std::cout << "(Should be Fire and Gas only)" << std::endl;

	std::cout << "* Fire...";
	if (events[0] != Event::Fire)
		throw ("Error expected Fire");
	std::cout << "ok!" << std::endl;

	std::cout << "* Gas...";
	if (events[1] != Event::Gas)
		throw ("Error expected Gas");
	std::cout << "ok!" << std::endl;

	if (events.size() != 2) // Should not register Event::Water
		throw ("Error: sensor detecting types it shouldn't");
	if (!(events[0] == Event::Fire && events[1] == Event::Gas))
		throw ("Error wrong auto events");



	/**
	*	Create plot event
	*/
	PrintHeader("Creating events from plotter");

	std::cout << "Adding Fire to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pBoat->CreatePlotEvent(Event::Fire, "Skyddsrum", "Trossdäck");
	
	std::cout << "Adding Water to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pBoat->CreatePlotEvent(Event::Water, "Skyddsrum", "Trossdäck");

	std::cout << "Adding Gas to 'Skyddsrum' in 'Trossdäck'" << std::endl;
	pBoat->CreatePlotEvent(Event::Gas, "Skyddsrum", "Trossdäck");



	/**
	*	Check plot event
	*/
	events = pBoat->GetEventsInRoom("Skyddsrum", "Trossdäck");
	std::cout << "\nChecking events in 'Skyddsrum' in 'Trossdäck':" << std::endl;

	if (events.size() != 3)
		throw ("Error unexpected number of plot events handled");

	std::string type;
	for (int i = 0; i < 3; i++)
	{
		type = Event::GetString((Event::Type)i);
		std::cout << "* " << type << "...";

		if (events[i] != (Event::Type)i)
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
	pBoat->SetEventLog(pEventLog);
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
	*	Clear event
	*/
	pBoat->ClearEvent(Event::Fire, "Skyddsrum", "Trossdäck");
	pBoat->ClearEvent(Event::Gas, "Skyddsrum", "Trossdäck");
	pBoat->ClearEvent(Event::Water, "Skyddsrum", "Trossdäck");
	


	/**
	*	Check cleared events
	*/
	events = pBoat->GetEventsInRoom("Skyddsrum", "Trossdäck");

	if (events.size() != 0)
		throw ("Error unexpected amount of events after clearing");



	/**
	*	Check eventlog
	*/
	int eventCount;			// Should be 5 (6 events -1 that didn't register)
	int activeEventCount;	// Should be 1 (one room is cleared)

	eventCount = pEventLog->GetEventCount();
	activeEventCount = pEventLog->GetActiveEventCount();

	if (eventCount != 5)
		throw ("Error unexpected amount of events in total");
	if (activeEventCount != 1)
		throw ("Error unexpected amount of active events");


	PrintHeader("Testing completed!");
	delete pBoat;
	delete pEventLog;
}

Boat* BoatTester::CreateBoat(EventLog *pEventLog) {
	PrintHeader("Building boat 'Testskepp'");

	Boat *newBoat = new Boat;
	newBoat->SetModelName("Testskepp");

	std::vector<Event::Type> inputs = {Event::Fire, Event::Gas};

	std::cout << "Adding deck 'Bryggdäck'...";
	newBoat->AddDeck("Bryggdäck");
	std::cout << "Adding deck 'Huvuddäck'...";
	newBoat->AddDeck("Huvuddäck");
	std::cout << "Adding deck 'Trossdäck'...";
	newBoat->AddDeck("Trossdäck");
	std::cout << "done!" << std::endl;

	std::cout << "\nAdding rooms to deck 'Bryggdäck'..." << std::endl;
	std::cout << "* slC...";
	newBoat->AddRoom("slC", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* SkyC...";
	newBoat->AddRoom("SkyC", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Brygga...";
	newBoat->AddRoom("Brygga", "Bryggdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "\nAdding rooms to deck 'Huvuddäck'..." << std::endl;
	std::cout << "* Skyddäck...";
	newBoat->AddRoom("Skyddäck", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång3...";
	newBoat->AddRoom("Gång3", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Tambur...";
	newBoat->AddRoom("Tambur", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Omformarrum...";
	newBoat->AddRoom("Omformarrum", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* CBRN...";
	newBoat->AddRoom("CBRN", "Huvuddäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "\nAdding rooms to deck 'Trossdäck'..." << std::endl;
	std::cout << "* Ammdurk...";
	newBoat->AddRoom("Ammdurk", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Lastrum...";
	newBoat->AddRoom("Lastrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* MC...";
	newBoat->AddRoom("MC", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång1...";
	newBoat->AddRoom("Gång1", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Apparatrum...";
	newBoat->AddRoom("Apparatrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång2...";
	newBoat->AddRoom("Gång2", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Hjälpmaskinrum...";
	newBoat->AddRoom("Hjälpmaskinrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Byssa...";
	newBoat->AddRoom("Byssa", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* SB Mäss...";
	newBoat->AddRoom("SB Mäss", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Skyddsrum...";
	newBoat->AddRoom("Skyddsrum", "Trossdäck", inputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	/**
	*	Send corresponding pointers
	*/
	std::cout << std::endl;

	std::cout << "\nAdding pointer from EventLog to Boat...";
	newBoat->SetEventLog(pEventLog);
	std::cout << "done!" << std::endl;

	return newBoat;
}

bool BoatTester::ReadFile(Boat *pBoat, std::string filePath)
{
	PrintHeader("Read boat from file");

	if (!pBoat->ReadFile(filePath))
		return false;

	return true;
}

void BoatTester::WriteFile(Boat *pBoat, std::string filePath)
{
	PrintHeader("Write boat to file");

	pBoat->WriteFile(filePath);
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
