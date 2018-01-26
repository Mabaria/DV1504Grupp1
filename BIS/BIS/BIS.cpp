// BIS.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "Boat.h"
#include "EventLog.h"
#include "ActiveLog.h"

void test();
void printHeader(std::string title);

int main()
{
	try
	{
		test();
	}
	catch(const char *e)
	{
		std::cout << e << std::endl;
	}

	getchar();
	
	return 0;
}

void test() {
	Boat myBoat;
	EventLog myEventLog;
	ActiveLog myActiveLog;

	/** 
	*	Create boat structure
	*/

	printHeader("Building boat 'Testskepp'");
	myBoat.SetModelName("Testskepp");

	std::cout << "Adding deck 'Bryggdäck'...";
	myBoat.AddDeck("Bryggdäck");
	std::cout << "Adding deck 'Huvuddäck'...";
	myBoat.AddDeck("Huvuddäck");
	std::cout << "Adding deck 'Trossdäck'...";
	myBoat.AddDeck("Trossdäck");
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Bryggdäck'..." << std::endl;
	std::cout << "* slC...";
	myBoat.AddRoom("slC", "Bryggdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* SkyC...";
	myBoat.AddRoom("SkyC", "Bryggdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Brygga...";
	myBoat.AddRoom("Brygga", "Bryggdäck");
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Huvuddäck'..." << std::endl;
	std::cout << "* Skyddäck...";
	myBoat.AddRoom("Skyddäck", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	myBoat.AddRoom("Maskinrum", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Gång3...";
	myBoat.AddRoom("Gång3", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Tambur...";
	myBoat.AddRoom("Tambur", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Omformarrum...";
	myBoat.AddRoom("Omformarrum", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "* CBRN...";
	myBoat.AddRoom("CBRN", "Huvuddäck");
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	std::cout << "Adding rooms to deck 'Trossdäck'..." << std::endl;
	std::cout << "* Ammdurk...";
	myBoat.AddRoom("Ammdurk", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Lastrum...";
	myBoat.AddRoom("Lastrum", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Maskinrum...";
	myBoat.AddRoom("Maskinrum", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* MC...";
	myBoat.AddRoom("MC", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Gång1...";
	myBoat.AddRoom("Gång1", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Apparatrum...";
	myBoat.AddRoom("Apparatrum", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Gång2...";
	myBoat.AddRoom("Gång2", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Hjälpmaskinrum...";
	myBoat.AddRoom("Hjälpmaskinrum", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Byssa...";
	myBoat.AddRoom("Byssa", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* SB Mäss...";
	myBoat.AddRoom("SB Mäss", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "* Skyddsrum...";
	myBoat.AddRoom("Skyddsrum", "Trossdäck");
	std::cout << "ok" << std::endl;
	std::cout << "done!" << std::endl;

	/**
	*	Send corresponding pointers
	*/
	std::cout << std::endl;

	std::cout << "Adding pointer from EventLog to Boat...";
	myBoat.SetEventLog(&myEventLog);
	std::cout << "done!" << std::endl;

	std::cout << "Adding pointer from ActiveLog to EventLog...";
	myEventLog.SetActiveLog(&myActiveLog);
	std::cout << "done!" << std::endl;

	/**
	*	Write boat structure to file
	*/
	std::cout << std::endl;

	std::cout << "Write boat structure to '/SaveFiles/BoatStructure.boat'...";
	myBoat.WriteFile("../../SaveFiles/BoatStructure.boat");
	std::cout << "done!" << std::endl;

	//myBoat.CreateEvent(Fire, "Maskinrum", "Bryggdäck");

	std::cout << "\n\nEnd of test." << std::endl;
}

void printHeader(std::string title)
{
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
}
