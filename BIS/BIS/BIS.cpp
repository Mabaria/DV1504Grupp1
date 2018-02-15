#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>

#include "../../Logic/Boat/Boat.h";

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	System system;
	system.BuildGraphicalUserInterface(L"BIS", 1920, 1080);
	system.Run();

	//EventLog myLog;
	//Boat myBoat;
	//
	//myBoat.SetEventLog(&myLog);
	//myBoat.ReadFile("../../Savefiles/data.boat");
	//
	//Room *myRoom = myBoat.GetRoomPointerAt(0);
	//myRoom->AddSensorEvent(Event::Fire);
	//myRoom->AddSensorEvent(Event::Water);
	//myRoom->AddSensorEvent(Event::Water);
	//myRoom->AddSensorEvent(Event::Fire);
	//myRoom->AddSensorEvent(Event::Water);
	//myRoom->AddSensorEvent(Event::Gas);
	//
	//std::vector<LogEvent*> myEvents = myRoom->GetActiveEvents();

	return 0;
}


