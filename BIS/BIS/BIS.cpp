// BIS.cpp : Defines the entry point for the console application.
//

#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "../../GraphicsEngine/Test_Graphics.h"
#include "../../UI/UIElements/Test_Panel.h"

int main()
{
	//Test_D3D11();
	Test_Panel3D();


	TestMesh();
//	TestAssimp();
	//TestBoat();
    return 0;
}


