#pragma once

#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "../../GraphicsEngine/Test_Button.h"
#include "../../GraphicsEngine/Camera/Test_Camera.h"
#include "../../GraphicsEngine/Test_Graphics.h"
#include "../../GraphicsEngine/Parsing/Test_Mesh.h"

#include "../../IO/Test_Input.h"
#include "../../IO/ObserverPattern/Test_Obs.h"

#include "../../UI/UIElements/Test_Panel.h"

namespace Testing
{
	bool testAll()
	{
		TestAssimp();
		TestBoat();
		Test_Button();
		TestCamera();
		Test_Window();
		Test_D3D11();
		TestMesh();
		Test_Input();
		Test_Observer();
		//Todo This test is blank
		Test_Panel2D();
		//Todo This test crashes
		//Test_Panel3D();

		return true;
	}

	bool testOne()
	{
		//! ADD YOUR OWN TESTS HERE
		return true;
	}
}