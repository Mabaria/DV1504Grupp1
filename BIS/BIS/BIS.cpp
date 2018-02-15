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
	//bool result;
	//
	//Room *myRoom0 = myBoat.GetRoomPointerAt(0);
	//result = myRoom0->AddSensorEvent(Event::Fire);
	//result = myRoom0->AddSensorEvent(Event::Water);
	//
	//Room *myRoom1 = myBoat.GetRoomPointerAt(1);
	//result = myRoom1->AddSensorEvent(Event::Fire);
	//result = myRoom1->AddSensorEvent(Event::Water);
	//
	//Room *myRoom2 = myBoat.GetRoomPointerAt(2);
	//result = myRoom2->AddSensorEvent(Event::Fire);
	//result = myRoom2->AddSensorEvent(Event::Water);
	//
	//result = myRoom1->ClearEvent(Event::Fire);
	//result = myRoom1->ClearEvent(Event::Water);
	//result = myRoom0->ClearEvent(Event::Fire);
	//result = myRoom0->ClearEvent(Event::Water);
	//result = myRoom2->ClearEvent(Event::Fire);
	//result = myRoom2->ClearEvent(Event::Water);
	//
	//std::vector<LogEvent*> myEvents0 = myRoom0->GetActiveEvents();
	//std::vector<LogEvent*> myEvents1 = myRoom1->GetActiveEvents();
	//std::vector<LogEvent*> myEvents2 = myRoom1->GetActiveEvents();

	return 0;
}


