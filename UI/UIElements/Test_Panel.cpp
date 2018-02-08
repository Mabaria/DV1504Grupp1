#include "Test_Panel.h"
#include "../../IO/Keyboard.h"
/*
	FÖR DEN SOM REVIEWAR DETTA

	Filerna som behöver kollas (utöver denna) är:
		Panel.h
		Panel.cpp
		Panel2D.h	(inte klar för det är inte möjligt innan textlådor och knappar är klara)
		Panel2D.cpp (--------------------------------||------------------------------------)
		Panel3D.h
		Panel3D.cpp
		MeshObject.h
		MeshObject.cpp
*/

void Test_Panel3D()
{
	Window window(L"Test_Window", 1280, 720);
	window.Open();
	Panel3D panel1(
		640,					// Width.
		360,					// Height.
		0,						// Top (y coordinate of top edge).
		0,						// Left (x coordinate of left edge).
		window.GetWindow(),		// Window handle to parent window of the panel.
		L"Test_Window");		// Name of the parent window.

	Panel3D panel2(640, 360, 0, 640, window.GetWindow(), L"Test_Window");


	int click_x = 960;
	int click_y = 30;

	// Panel::Intersects returns a bool that states whether or not the click intersects the panel.
	bool click_intersects_panel1 = panel1.Intersects(click_x, click_y); // false due to the x coordinate being outside of panel1's range. 
	bool click_intersects_panel2 = panel2.Intersects(click_x, click_y); // true.

	std::cout << "Panel 1 intersection bool: " << std::boolalpha << click_intersects_panel1 << std::endl;
	std::cout << "Panel 2 intersection bool: " << std::boolalpha << click_intersects_panel2 << std::endl;


	// Panel::IntersectionFraction (Emil's function). Check function definition to understand.
	Fraction frac1 = panel1.IntersectionFraction(click_x, click_y); 
	Fraction frac2 = panel2.IntersectionFraction(click_x, click_y);

	std::cout << "Panel 1 x fraction: " << frac1.x << std::endl;
	std::cout << "Panel 1 y fraction: " << frac1.y << std::endl;

	std::cout << "Panel 2 x fraction: " << frac2.x << std::endl;
	std::cout << "Panel 2 y fraction: " << frac2.y << std::endl;

	
	// Testing shader creation and setting as well as setting up the IA and drawing.

	// Triangle vertices for drawing and mesh object test.
	std::vector<std::vector<Vertex>> vertices;
	
	Vertex v1 = {
		-0.5f, -0.5f,  0.5f,	// Position x, y, z
		 0.0f,  0.0f,  0.0f,	// Normal x, y, z
		 0.0f,  0.0f			// TexCoord u, v
	};
	Vertex v2 = {
		 0.0f,  0.5f,  0.5f,
		 0.0f,  0.0f,  0.0f,
		 0.0f,  0.0f
	};
	Vertex v3 = {
		 0.5f, -0.5f,  0.5f,
		 0.0f,  0.0f,  0.0f,
		 0.0f,  0.0f
	};

	std::vector<Vertex> vert1;
	vert1.push_back(v1);
	vert1.push_back(v2);
	vert1.push_back(v3);

	vertices.push_back(vert1);

	// A vector of vectors because Rikard.
	std::vector<std::vector<unsigned int>> indices;

	std::vector<unsigned int> i1;
	i1.push_back(0);
	i1.push_back(1);
	i1.push_back(2);

	indices.push_back(i1);

	//panel1.AddMeshObject("tri", indices, vertices);
	//panel2.AddMeshObject("tri", indices, vertices);
	//MeshObject *panel1_tri = panel1.rGetMeshObject("tri");
	//MeshObject *panel2_tri = panel2.rGetMeshObject("tri");

	Camera camera({ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, 1.0f, 1.0f, 0.1f, 10000.0f, LOOK_AT, PERSPECTIVE);

	panel1.SetCamera(&camera);
	panel2.SetCamera(&camera);

	panel1.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	panel2.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	float i = 0.1f;
	while (window.IsOpen())
	{
		window.Update();
		panel1.Draw();
		panel2.Draw();
		//panel1_tri->Rotate(0, 0, i);
		panel1.Update();
		//panel2_tri->Rotate(i, 0, 0);
		panel2.Update();
	}	
}

void Test_Panel2D()
{
	int window_height = 720;
	int window_width = 1280;

	Window window(L"Button_Test", window_width, window_height);
	Panel2D testPanel(
		window_width / 6,		// Width
		window_height,			// Height
		0,						// Top
		5 * window_width / 6,	// Left
		window.GetWindow(), 
		L"Button_Test");
	Panel2D testHeadPanel(
		window_width / 2, 
		window_height / 6, 
		0, 
		window_width / 3, 
		window.GetWindow(), 
		L"Button_Test");
	Panel3D testHead3DPanel(
		window_width / 3, 
		window_height / 6, 
		0, 
		0, 
		window.GetWindow(), 
		L"Button_Test");
	Panel3D test3DPanel(
		5 * window_width / 6, 
		5 * window_height / 6, 
		window_height / 6, 
		0, 
		window.GetWindow(), L"Button_Test");

	testPanel.AddButton(100, 100, 20, 20, "../../Models/FireButton.png", "FireButton");
	testPanel.AddButton(100, 100, 120, 20, "../../Models/FireButton.png", "FireButton2");
	testPanel.AddButton(100, 100, 220, 20, "../../Models/FireButton.png", "FireButton3");
	testHeadPanel.AddButton(100, 100, 20, 20, "../../Models/Fern.jpg", "FernButton");
	testHeadPanel.AddButton(200, 200, 20, 400, "../../Models/pepehands.jpg", "FernButton");
	testHeadPanel.AddButton(200, 200, 20, 700, "../../Models/feelsrain.gif", "FernButton");
	window.Open();
	testPanel.GetButtonByIndex(1); // Expected: FireButton2
	testPanel.GetButtonByName("FireButton3")->SetButtonsize(-220, 320, 140, 420);
	/* Expected: Button 3 is displaced and stretches a bit*/
	testPanel.GetButtonByIndex(-1); // Expected: nullptr
	testPanel.GetButtonByIndex(3285); // Expected: nullptr
	testHeadPanel.GetButtonByName("Wesseboii"); // Expected: nullptr



	Mesh testMesh("../../Models/OBJTEST2.obj");
	test3DPanel.CreateShadersAndSetup(L"../../GraphicsEngine/Test_VertexShader.hlsl", L"", L"../../GraphicsEngine/Test_PixelShader.hlsl");
	test3DPanel.AddMeshObject("Test", testMesh.GetIndexVectors(), testMesh.GetVertexVectors(), L"");
	test3DPanel.rGetMeshObject("Test")->Scale(1.0f, 1.0f, 1.0f);
	Camera testCamera(0.0f, 50.0f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
	test3DPanel.SetCamera(&testCamera);

	Mesh testMesh2("../../Models/OBJTEST2.obj");
	testHead3DPanel.CreateShadersAndSetup(L"../../GraphicsEngine/Test_VertexShader.hlsl", L"", L"../../GraphicsEngine/Test_PixelShader.hlsl");
	testHead3DPanel.AddMeshObject("Test2", testMesh2.GetIndexVectors(), testMesh2.GetVertexVectors(), L"");
	Camera testCamera2(0.0f, 50.0f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
	testHead3DPanel.SetCamera(&testCamera2);
	// testHeadPanel.GetButtonByName("FernButton")->AddObserver(&window);
	// Doesnt work because there's no class that inherits observer yet
	// But shows how you would do to add functionality to a button

	while (window.IsOpen())
	{
		window.Update();
		testPanel.Update();
		testHeadPanel.Update();
		test3DPanel.Update();
		testHead3DPanel.Update();
		testPanel.Draw();
		testHeadPanel.Draw();
		test3DPanel.Draw();
		testHead3DPanel.Draw();

	}
}

void Test_BoatOnScreen()
{
	Mesh floor2("../../Models/Floor2.obj");
	Mesh floor1("../../Models/Floor1.obj");
	Mesh floor01("../../Models/Floor01.obj");

	MeshObject floor2_object ("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors());
	MeshObject floor1_object ("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors());
	MeshObject floor01_object("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors()); 

	std::wstring window_name = L"Demo_BIS";

	Window window(window_name, 1280, 720);

	Panel3D side_view(1280 / 3, 720 / 3, 0, 0, window.GetWindow(), window_name.c_str());
	Panel3D top_view(2 * 1280 / 3, 2 * 720 / 3, 720 / 3, 0, window.GetWindow(), window_name.c_str());

	// Hela båten tjoff in i panelerna och sen mixtras matriserna med.
	side_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"");
	side_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"");
	side_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"");

	top_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"");
	top_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"");
	top_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"");

	top_view.rGetMeshObject("floor2")->Scale	(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor1")->Scale	(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor01")->Scale	(0.1f, 0.1f, 0.1f);

	top_view.rGetMeshObject("floor2")->Translate	(0.0f, 0.0f, 0.5f);
	top_view.rGetMeshObject("floor1")->Translate	(0.0f, 0.0f, 0.0f);
	top_view.rGetMeshObject("floor01")->Translate	(0.0f, 0.0f, -0.5f);
	
	side_view.rGetMeshObject("floor2")->Scale	(0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor1")->Scale	(0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor01")->Scale	(0.15f, 0.4f, 0.1f);
	
	side_view.rGetMeshObject("floor2")->Translate	(0.05f, -0.2f, 0.0f);
	side_view.rGetMeshObject("floor1")->Translate	(0.05f, 0.0f, 0.0f);
	side_view.rGetMeshObject("floor01")->Translate	(0.05f, 0.2f, 0.0f);

	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera2(
		{ 0.0f, 0.0f, -0.2f, 0.0f }, 
		{ 0.0f, 1.0f, 0.0f, 0.0f }, 
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		2.0f, 2.0f,
		0.1f, 100.0f, LOOK_AT, ORTHOGRAPHIC);

	Camera camera(
		{ 0.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 8.0f, 16.0f / 9.0f,
		0.1f, 1000.0f, LOOK_AT, PERSPECTIVE);

	side_view.SetCamera(&camera2);
	top_view.SetCamera(&camera);

	float speed = 0.1f;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		
		if (Keyboard::IsKeyDown(Keys::W))
		{
			camera.MoveCamera(0.0f, 0.0f, 1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::S))
		{
			camera.MoveCamera(0.0f, 0.0f, -1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::D))
		{
			camera.MoveCamera(1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::A))
		{
			camera.MoveCamera(-1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Space))
		{
			camera.MoveCamera(0.0f, 1.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Shift))
		{
			camera.MoveCamera(0.0f, -1.0f, 0.0f, speed);
		}

		if (Keyboard::IsKeyPressed(Keys::Esc))
		{
			window.Close();
		}
		side_view.Update();
		top_view.Update();

		side_view.Draw();
		top_view.Draw();
	}	
}

void Test_Panel2DTextBoxes()
{
	Mesh floor2("../../Models/Floor2.obj");
	Mesh floor1("../../Models/Floor1.obj");
	Mesh floor01("../../Models/Floor01.obj");

	MeshObject floor2_object("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors());
	MeshObject floor1_object("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors());
	MeshObject floor01_object("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors());

	std::wstring window_name = L"Testboi";
	int window_height = 720;
	int window_width = 1280;

	Window window(L"Button_Test", window_width, window_height);

	Panel3D side_view(
		window_width / 3,
		window_height / 6,
		0,
		0,
		window.GetWindow(),
		L"Button_Test");
	Panel3D top_view(
		5 * window_width / 6,
		5 * window_height / 6,
		window_height / 6,
		0,
		window.GetWindow(), L"Button_Test");

	// Hela båten tjoff in i panelerna och sen mixtras matriserna med.
	side_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"");
	side_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"");
	side_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"");

	top_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"");
	top_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"");
	top_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"");

	top_view.rGetMeshObject("floor2")->Scale(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor1")->Scale(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor01")->Scale(0.1f, 0.1f, 0.1f);

	top_view.rGetMeshObject("floor2")->Translate(0.0f, 0.0f, 0.5f);
	top_view.rGetMeshObject("floor1")->Translate(0.0f, 0.0f, 0.0f);
	top_view.rGetMeshObject("floor01")->Translate(0.0f, 0.0f, -0.5f);

	side_view.rGetMeshObject("floor2")->Scale( 0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor1")->Scale( 0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor01")->Scale(0.15f, 0.4f, 0.1f);

	side_view.rGetMeshObject("floor2")->Translate(0.05f, -0.2f, 0.0f);
	side_view.rGetMeshObject("floor1")->Translate(0.05f, 0.0f, 0.0f);
	side_view.rGetMeshObject("floor01")->Translate(0.05f, 0.2f, 0.0f);

	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera2(
		{ 0.0f, 0.0f, -0.2f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		2.0f, 2.0f,
		0.1f, 100.0f, LOOK_AT, ORTHOGRAPHIC);

	Camera camera(
		{ 0.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 8.0f, 16.0f / 9.0f,
		0.1f, 1000.0f, LOOK_AT, PERSPECTIVE);

	side_view.SetCamera(&camera2);
	top_view.SetCamera(&camera);

	Panel2D log_panel(
		window_width / 6,		// Width
		window_height,			// Height
		0,						// Top
		5 * window_width / 6,	// Left
		window.GetWindow(),
		L"Button_Test");
	Panel2D control_panel(
		window_width / 2,
		window_height / 6,
		0,
		window_width / 3,
		window.GetWindow(),
		L"Button_Test");

	log_panel.AddButton(100, 100, 20, 20, "../../Models/Button01.png", "Dead");
	log_panel.AddButton(100, 100, 120, 20, "../../Models/Button02.png", "Gas");
	log_panel.AddButton(100, 100, 220, 20, "../../Models/Button03.png", "Water");
	log_panel.AddButton(100, 100, 320, 20, "../../Models/Button04.png", "Fire");

	float speed = 0.1f;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();

		if (Keyboard::IsKeyDown(Keys::W))
		{
			camera.MoveCamera(0.0f, 0.0f, 1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::S))
		{
			camera.MoveCamera(0.0f, 0.0f, -1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::D))
		{
			camera.MoveCamera(1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::A))
		{
			camera.MoveCamera(-1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Space))
		{
			camera.MoveCamera(0.0f, 1.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Shift))
		{
			camera.MoveCamera(0.0f, -1.0f, 0.0f, speed);
		}

		if (Keyboard::IsKeyPressed(Keys::Esc))
		{
			window.Close();
		}
		side_view.Update();
		top_view.Update();
		control_panel.Update();
		log_panel.Update();

		side_view.Draw();
		top_view.Draw();
		control_panel.Draw();
		log_panel.Draw();
	}
}
