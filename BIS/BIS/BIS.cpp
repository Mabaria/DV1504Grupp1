#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>

#include "../../Logic/BoatTester.h"

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//System system;
	//system.BuildGraphicalUserInterface(L"BIS", 1920, 1014);
	//system.Run();

	BoatTester::TestBoat();
	getchar();
	
	return 0;
}


