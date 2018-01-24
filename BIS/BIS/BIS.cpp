// BIS.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "Boat.h"
#include "EventLog.h"
#include "ActiveLog.h"

void test();

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

	myBoat.AddDeck("Bryggdäck", 8);
}