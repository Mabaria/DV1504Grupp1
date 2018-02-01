#include "BoatTester.h"

void BoatTester::TestBoat()
{
	ActiveLog *mActiveLog;
	EventLog *mEventLog;
	Boat *mBoat;

	mActiveLog = new ActiveLog;
	mEventLog = CreateEventLog(mActiveLog);

	/**
	*	Create boat
	*/
	mBoat = CreateBoat(mEventLog);

	/**
	*	Write boat to file
	*/
	WriteFile(mBoat, "../../SaveFiles/Testboat1.boat");

	/**
	*	Read boat from file
	*/
	delete mBoat;
	mBoat = new Boat;
	mBoat->SetEventLog(mEventLog);
	if (!ReadFile(mBoat, "../../SaveFiles/Testboat1.boat"))
	{
		throw ("Error reading file");
	}

	/**
	*	Write boat to a new file
	*/
	WriteFile(mBoat, "../../SaveFiles/Testboat2.boat");

	/**
	*	Compare the two files
	*/
	if (!CompareFiles("../../SaveFiles/Testboat1.boat", "../../SaveFiles/Testboat2.boat"))
	{
		throw ("Error missmatching files from Write and Read");
	}

	/**
	*	Create auto event
	*/
	this->mBoat->CreateAutoEvent(Event::Fire, "Maskingrum", "Huvuddäck");

	/**
	*	Check auto event
	*/

	/**
	*	Create plot event
	*/
	this->mBoat->CreatePlotEvent(Event::Water, "Skyddsrum", "Trossdäck");

	/**
	*	Check plot event
	*/

	/**
	*	Check eventlog
	*/


	delete mBoat;
	delete mEventLog;
	delete mActiveLog;
}

Boat* BoatTester::CreateBoat(EventLog *pEventLog) {
	printHeader("Building boat 'Testskepp'");

	Boat *newBoat = new Boat;
	newBoat->SetModelName("Testskepp");

	const int nrOfInputs = 2;
	int inputs[nrOfInputs] = {0, 2};

	std::cout << "Adding deck 'Bryggdäck'...";
	newBoat->AddDeck("Bryggdäck");
	std::cout << "Adding deck 'Huvuddäck'...";
	newBoat->AddDeck("Huvuddäck");
	std::cout << "Adding deck 'Trossdäck'...";
	newBoat->AddDeck("Trossdäck");
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Bryggdäck'..." << std::endl;
	std::cout << "* slC...";
	newBoat->AddRoom("slC", "Bryggdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* SkyC...";
	newBoat->AddRoom("SkyC", "Bryggdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Brygga...";
	newBoat->AddRoom("Brygga", "Bryggdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Huvuddäck'..." << std::endl;
	std::cout << "* Skyddäck...";
	newBoat->AddRoom("Skyddäck", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång3...";
	newBoat->AddRoom("Gång3", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Tambur...";
	newBoat->AddRoom("Tambur", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Omformarrum...";
	newBoat->AddRoom("Omformarrum", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* CBRN...";
	newBoat->AddRoom("CBRN", "Huvuddäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Trossdäck'..." << std::endl;
	std::cout << "* Ammdurk...";
	newBoat->AddRoom("Ammdurk", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Lastrum...";
	newBoat->AddRoom("Lastrum", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	newBoat->AddRoom("Maskinrum", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* MC...";
	newBoat->AddRoom("MC", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång1...";
	newBoat->AddRoom("Gång1", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Apparatrum...";
	newBoat->AddRoom("Apparatrum", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Gång2...";
	newBoat->AddRoom("Gång2", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Hjälpmaskinrum...";
	newBoat->AddRoom("Hjälpmaskinrum", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Byssa...";
	newBoat->AddRoom("Byssa", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* SB Mäss...";
	newBoat->AddRoom("SB Mäss", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "* Skyddsrum...";
	newBoat->AddRoom("Skyddsrum", "Trossdäck", inputs, nrOfInputs);
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	/**
	*	Send corresponding pointers
	*/
	std::cout << std::endl;

	std::cout << "Adding pointer from EventLog to Boat...";
	newBoat->SetEventLog(pEventLog);
	std::cout << "done!" << std::endl;

	return newBoat;
}

EventLog* BoatTester::CreateEventLog(ActiveLog *pActiveLog)
{
	printHeader("Creating Event log");

	EventLog *newEventLog = new EventLog;
	std::cout << "Adding pointer from ActiveLog to EventLog...";
	newEventLog->SetActiveLog(pActiveLog);
	std::cout << "done!" << std::endl;

	return newEventLog;

}

bool BoatTester::ReadFile(Boat *pBoat, std::string filePath)
{
	printHeader("Read boat from file");

	if (!pBoat->ReadFile(filePath))
		return false;

	return true;
}

void BoatTester::WriteFile(Boat *pBoat, std::string filePath)
{
	printHeader("Write boat to file");

	pBoat->WriteFile(filePath);
}

bool BoatTester::CompareFiles(std::string filePath1, std::string filePath2)
{
	printHeader("Comparing files");

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

void BoatTester::printHeader(std::string title)
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
