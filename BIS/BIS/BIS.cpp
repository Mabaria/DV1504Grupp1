#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>
#include "../../Logic/Boat/Boat.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	System system;
	system.BuildGraphicalUserInterface(L"BIS", 1920, 1014);
	system.Run();

	//EventLog myLog;
	//Boat myBoat;
	//
	//myBoat.SetEventLog(&myLog);
	//myBoat.ReadFile("../../SaveFiles/data.boat");
	
	return 0;
}


