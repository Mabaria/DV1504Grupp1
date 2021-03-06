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
	system.BuildGraphicalUserInterface(L"BIS", 1920, 1080);

	// TO change simulation, change these values
	float chance	= 1.0f;					// For an event to happen [0,1]
	int	seconds		= 3;					// Seconds between a new chance
	Simulator sim(system, chance, seconds);
	
	while (program_open)
	{
		program_open = system.Run();
		sim.Update();
	}
	
	return 0;
}


