// BIS.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
#include "../../GraphicsEngine/Parsing/Mesh.h"
//#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "../../GraphicsEngine/Camera/Test_Camera.h"

#include "../../Logic/BoatTester.h"

int main()
{
//	TestMesh();
//	TestAssimp();
	//TestCamera();


	try
	{
		BoatTester::TestBoat();
	}
	catch (const char* e)
	{
		std::cout << e << std::endl;
	}

	getchar();

	return 0;
}


