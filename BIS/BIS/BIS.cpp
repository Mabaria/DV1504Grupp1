#include "../../Test/Testing.h"
#include "../BIS/System/System.h"
#include <crtdbg.h>
#include "../../Logic/Boat/Boat.h"

#include "../../Simulation/Simulator.h"

int main()
{
	bool program_open = true;

	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	System system;
	system.BuildGraphicalUserInterface(L"BIS", 1920, 1014);

	Simulator sim(system, 0.5f, 3);
	
	while (program_open)
	{
		program_open = system.Run();
		sim.Update();
	}
	
	return 0;
}


