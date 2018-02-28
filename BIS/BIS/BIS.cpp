#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>

#include "../../Logic/Test_Log.h"

#include "../../Logic/BoatTester.h"
#include <iostream>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//System system;
	//system.BuildGraphicalUserInterface(L"BIS", 1920, 1014);
	//system.Run();

	//bool flag = Test_Log::MainTest();
	try
	{
		BoatTester::TestBoat();
	}
	catch (const char *e)
	{
		std::cout << e << std::endl;
	}

	getchar();
	
	return 0;
}


