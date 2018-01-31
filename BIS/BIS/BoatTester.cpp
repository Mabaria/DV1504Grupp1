#include "BoatTester.h"

void TestBoat()
{
	ActiveLog *mActiveLog;
	EventLog *mEventLog;
	Boat *mBoat;

	mActiveLog = new ActiveLog;
	mEventLog = CreateEventLog(mActiveLog);
	mBoat = CreateBoat(mEventLog);

	ReadFile(mBoat, "../../SaveFiles/BoatStructure.boat");
	WriteFile(mBoat, "../../SaveFiles/newBoat.boat");

	CompareFiles("../../SaveFiles/BoatStructure.boat", "../../SaveFiles/newBoat.boat");

	printHeader("End of test");

	getchar();

	delete mBoat;
	delete mEventLog;
	delete mActiveLog;
}

Boat* CreateBoat(EventLog *pEventLog) {
	printHeader("Building boat 'Testskepp'");

	Boat *newBoat = new Boat;
	newBoat->SetModelName("Testskepp");

	int inputs[3] = {0, 1, 2};

	std::cout << "Adding deck 'Bryggdäck'...";
	newBoat->AddDeck("Bryggdäck");
	std::cout << "Adding deck 'Huvuddäck'...";
	newBoat->AddDeck("Huvuddäck");
	std::cout << "Adding deck 'Trossdäck'...";
	newBoat->AddDeck("Trossdäck");
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Bryggdäck'..." << std::endl;
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

	std::cout << "Adding rooms to deck 'Huvuddäck'..." << std::endl;
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

	std::cout << "Adding rooms to deck 'Trossdäck'..." << std::endl;
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

	std::cout << "Adding pointer from EventLog to Boat...";
	newBoat->SetEventLog(pEventLog);
	std::cout << "done!" << std::endl;

	return newBoat;
}

EventLog* CreateEventLog(ActiveLog *pActiveLog)
{
	printHeader("Creating Event log");

	EventLog *newEventLog = new EventLog;
	std::cout << "Adding pointer from ActiveLog to EventLog...";
	newEventLog->SetActiveLog(pActiveLog);
	std::cout << "done!" << std::endl;

	return newEventLog;

}

void ReadFile(Boat *pBoat, std::string filePath)
{
	printHeader("Testing: Read boat from file");

	std::cout << "Reading boat structure from '" << filePath << "'...";
	pBoat->ReadFile(filePath);
	std::cout << "done!" << std::endl;
}

void WriteFile(Boat *pBoat, std::string filePath)
{
	printHeader("Testing: Write boat to file");

	std::cout << "Write boat structure to '/SaveFiles/BoatStructure.boat'...";
	try
	{
		pBoat->WriteFile(filePath);
	}
	catch(const char *e)
	{
		throw (e);
	}
	std::cout << "done!" << std::endl;
}

bool CompareFiles(std::string filePath1, std::string filePath2)
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

		//gotLine1 = getline(file1, line1);
		//gotLine2 = getline(file2, line2);

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

void printHeader(std::string title)
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
