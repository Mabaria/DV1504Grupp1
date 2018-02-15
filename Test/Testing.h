#pragma once

#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "../../GraphicsEngine/Test_Button.h"
#include "../../GraphicsEngine/Camera/Test_Camera.h"
#include "../../GraphicsEngine/Test_Graphics.h"
#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
#include "../../GraphicsEngine/Parsing/Test_MaterialHandler.h"

#include "../IO/Test_Input.h"
#include "../IO/ObserverPattern/Test_Obs.h"

#include "../../IO/Test_Input.h"
#include "../../IO/ObserverPattern/Test_Obs.h"
#include "../../IO/Test_Picking.h"
#include "../UI/UIElements/Test_Panel.h"
#include "../UI/UIElements/Test_TextBox.h"

#include "../Logic/BoatTester.h"
#include "../UI/GUI/EventMenu.h"

namespace Testing
{
	bool testAll()
	{
		//TestAssimp();
		//TestBoat();
		//Test_Button();
		//TestCamera();
		//Test_Window();
		//Test_D3D11();
		//TestMesh();
		//Test_Input();
		//Test_Observer();
		//Test_Panel2D();
		//Todo This test crashes
		//Test_Panel3D();
		//Test_BoatOnScreen();
		//Test_Panel2DTextBoxes();

		
		return true;
	}

	bool testOne()
	{
		int window_width = 1280;
		int window_height = 720;
		Window window(L"EventMenu", window_width, window_height);
		EventLog eventLog;

		EventMenu eventMenu;
		eventMenu.Init(
			window.GetClientSize().width,
			window.GetClientSize().height,
			&eventLog,
			L"EventMenu",
			&window);

		window.Open();
		while (window.IsOpen())
		{
			window.Update();
			eventMenu.Update();
			eventMenu.Draw();
		}
		return true;
	}
}