#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>

#include "../../Logic/Boat/Boat.h"
#include "../../Logic/Event/Event.h"
#include <iostream>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//System system;
	//system.BuildGraphicalUserInterface(L"BIS", 1920, 1014);
	//system.Run();

	EventLog myLog;
	Boat myBoat;
	
	myBoat.SetEventLog(&myLog);
	myBoat.ReadFile("../../SaveFiles/data.boat");

	Room* myRoom;
	Event::Type type;

	for (int i = 0; i < 19; i++)
	{
		myRoom = myBoat.GetRoomPointerAt(i);
		type = (Event::Type) (i % 4);

		myRoom->AddPlotterEvent(type);

		std::cout << "Added " << Event::GetString(type) << " to room " << i+1 << "/19" << std::endl;
		
		for (int j = 0; j < 200000000; j++);
	}

	myLog.SaveToFile("../../Savefiles/logfile.log");
	
	return 0;
}


