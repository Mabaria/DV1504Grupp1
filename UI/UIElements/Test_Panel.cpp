#include "Test_Panel.h"
#include "../../IO/Keyboard.h"
/*
	F�R DEN SOM REVIEWAR DETTA

	Filerna som beh�ver kollas (ut�ver denna) �r:
		Panel.h
		Panel.cpp
		Panel2D.h	(inte klar f�r det �r inte m�jligt innan textl�dor och knappar �r klara)
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

	panel1.AddMeshObject("tri", indices, vertices);
	panel2.AddMeshObject("tri", indices, vertices);
	MeshObject *panel1_tri = panel1.rGetMeshObject("tri");
	MeshObject *panel2_tri = panel2.rGetMeshObject("tri");

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
		panel1_tri->Rotate(0, 0, i);
		panel1.Update();
		panel2_tri->Rotate(i, 0, 0);
		panel2.Update();
	}	
}

void Test_Panel2D()
{
	Window window(L"Button_Test", 1280, 720);
	Panel2D testPanel(400, 1080, 0, 1520, window.GetWindow(), L"Button_Test");
	Panel2D testHeadPanel(1120, 200, 0, 400, window.GetWindow(), L"Button_Test");
	Panel3D testHead3DPanel(399, 200, 0, 0, window.GetWindow(), L"Button_Test");
	Panel3D test3DPanel(1520, 880, 200, 0, window.GetWindow(), L"Button_Test");
	testPanel.AddButton(100, 100, 20, 20, "../../Models/FireButton.png", "FireButton");
	testPanel.AddButton(100, 100, 120, 20, "../../Models/FireButton.png", "FireButton2");
	testPanel.AddButton(100, 100, 220, 20, "../../Models/FireButton.png", "FireButton3");
	testHeadPanel.AddButton(100, 100, 20, 20, "../../Models/Fern.jpg", "FernButton");
	testHeadPanel.AddButton(200, 200, 20, 400, "../../Models/pepehands.jpg", "FernButton");
	testHeadPanel.AddButton(200, 200, 20, 700, "../../Models/feelsrain.gif", "FernButton");
	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		testPanel.Update();
		testHeadPanel.Update();
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

	std::wstring window_name = L"Demo_BIS";

	Window window(window_name, 1280, 720);

	Panel3D side_view(1280 / 3, 720 / 3, 0, 0, window.GetWindow(), window_name.c_str());

	/*side_view.AddMeshObject(
		"floor01",
		floor01.GetIndexVectors(),
		floor01.GetVertexVectors());*/
	/*side_view.AddMeshObject(
		"floor1",
		floor1.GetIndexVectors(),
		floor1.GetVertexVectors());*/

	Panel3D top_view(2 * 1280 / 3, 2 * 720 / 3, 720 / 3, 0, window.GetWindow(), window_name.c_str());

	side_view.AddMeshObject(
		"floor1",
		floor1.GetIndexVectors(),
		floor1.GetVertexVectors());
	top_view.AddMeshObject(
		"floor2",
		floor2.GetIndexVectors(),
		floor2.GetVertexVectors());

	side_view.rGetMeshObject("floor1")->Scale(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor2")->Scale(0.3f, 0.3f, 0.3f);

	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera2(
		{ 0.0f, 5.0f, 3.5f, 0.0f }, 
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
			//top_view.rGetMeshObject("floor2")->Rotate(0.0f, 0.01f, 0.0f);
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
