#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	System system;
	system.BuildGraphicalUserInterface(L"BIS", 1280, 720);
	system.Run();
    return 0;
}


