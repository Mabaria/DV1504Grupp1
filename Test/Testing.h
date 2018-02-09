#pragma once

#include "../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../GraphicsEngine/Parsing/Test_Boat.h"
#include "../GraphicsEngine/Test_Button.h"
#include "../GraphicsEngine/Camera/Test_Camera.h"
#include "../GraphicsEngine/Test_Graphics.h"
#include "../GraphicsEngine/Parsing/Test_Mesh.h"

#include "../IO/Test_Input.h"
#include "../IO/ObserverPattern/Test_Obs.h"

#include "../../IO/Test_Input.h"
#include "../../IO/ObserverPattern/Test_Obs.h"
#include "../../IO/Test_Picking.h"
#include "../UI/UIElements/Test_Panel.h"
#include "../UI/UIElements/Test_TextBox.h"

#include "../Logic/BoatTester.h"
#include "../../Logic/Boat/Boat.h"

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

		Test_Panel2DTextBoxes();
		return true;
	}

	bool testOne()
	{
		//! ADD YOUR OWN TESTS HERE
		Boat boat;
		boat.LoadBoatMesh();
		boat.LoadBoundingBoxes();

		Window window(L"HELLO", 800, 600);

		Panel3D panel(800, 600, 0, 0, window.GetWindow(), L"HELLO");

		//panel.AddMeshObject(boat.GetDeckMeshObject(0));
		//panel.AddMeshObject(boat.GetDeckMeshObject(1));
		panel.AddMeshObject(boat.GetDeckMeshObject(2));

		panel.CreateShadersAndSetup(L"../../GraphicsEngine/Test_VertexShader.hlsl", L"", L"../../GraphicsEngine/Test_PixelShader.hlsl");

		Camera camera(
			0.0f, 10.0f, -4.f,
			0.f, 1.f, 0.f,
			0.0f, 0.0f, 0.0f, 90.f, 1.33f, 0.1f, 1000.f);

		panel.SetCamera(&camera);

		window.Open();
		while (window.IsOpen())
		{
			window.Update();
			panel.Update();
			panel.Draw();
			Ray ray;
			Picking::GetWorldRay(&camera, 0.5f, 0.5f, ray);
			Room *room = boat.GetPickedRoom(ray);
			if (room != nullptr)
				std::cout << "HIT ROOM: " << room->GetName() << std::endl;
		}

		return true;
	}
}