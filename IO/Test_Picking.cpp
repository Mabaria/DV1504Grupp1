#include "Test_Picking.h"

bool TestPickingOnBoat()
{
	//! ADD YOUR OWN TESTS HERE

	// Read and create rooms/decks from file
	Boat boat;
	boat.ReadFile("../../Savefiles/data.boat");

	// Create window with panel
	Window window(L"HELLO", 1920, 1080);

	Panel3D panel(1920, 1080, 0, 0, window.GetWindow(), L"HELLO");

	width = 100;
	height = 100;
	nearZ = 50;
	farZ = 500;
	fov = (float)(2 * std::atanf(width / (2 * nearZ)) * 180 / PI);
	aspect = width / height;
	// Create shaders
	panel.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	// Load meshes into list
	Mesh meshList[6] = {
		Mesh("../../Models/Bounding/Bound01.obj"),
		Mesh("../../Models/Bounding/Bound1.obj"),
		Mesh("../../Models/Bounding/Bound2.obj"),
		Mesh("../../Models/DeckMesh/Floor01.obj"),
		Mesh("../../Models/DeckMesh/Floor1.obj"),
		Mesh("../../Models/DeckMesh/Floor2.obj")
	};
	
	// Define names for MeshObjects
	std::string meshNames[6] = {
		"Bound01",
		"Bound1",
		"Bound2",
		"Floor01",
		"Floor1",
		"Floor2" };

	// Creates MeshObjects from the meshes add adds them to panel
	for (int i = 0; i < 6; i++)
	{
		panel.AddMeshObject(&MeshObject(meshNames[i], &meshList[i]));
	}

	// Translate the first and third floor (with bounding boxes)
	panel.rGetMeshObject("Bound01")->Translate(0.0f, 0.0f, 3.5f);
	panel.rGetMeshObject("Floor01")->Translate(0.0f, 0.0f, 3.5f);
	panel.rGetMeshObject("Bound2")->Translate(0.0f, 0.0f, -3.5f);
	panel.rGetMeshObject("Floor2")->Translate(0.0f, 0.0f, -3.5f);

	// Get reference to the world matrices
	DirectX::XMMATRIX *floorMatrixList[3] = {
		panel.rGetMeshObject("Floor01")->rGetModelMatrix(),
		panel.rGetMeshObject("Floor1")->rGetModelMatrix(),
		panel.rGetMeshObject("Floor2")->rGetModelMatrix()
	};

	// Loads the first three meshes from the list and makes bounding boxes
	boat.LoadBoundingBoxes(meshList, floorMatrixList, 3);

	/**
	*	Create camera object
	*/
	// --- PERSPECTIVE ---
	Camera camera(
		{ 2.0f, 3.0f, 3.5f, 0.0f },		// Pos
		{ 0.0f, 1.0f, 0.0f, 0.0f },		// Up
		{ 0.0f, 0.0f, 0.0f, 0.0f },		// Dir
		XM_PI / 15.0f, 16.0f / 9.0f,	// With, Height
		0.1f, 100.0f,					// Near, Far
		LOOK_AT, PERSPECTIVE);			// Modes

	// --- ORTHOGRAPHIC ---
	//Camera camera(
	//	{ 0.0f, 30.0f, 0.0f, 0.0f },	// Pos
	//	{ 0.0f, 0.0f, 1.0f, 0.0f },		// Up
	//	{ 0.0f, -1.0f, 0.0f, 0.0f },	// Dir
	//	19.2f, 10.8f,					// With, Height
	//	0.1f, 100.0f,					// Near, Far
	//	LOOK_TO, ORTHOGRAPHIC);			// Modes

	// Set camera
	panel.SetCamera(&camera);

	window.Open();

	Ray ray;

	while (window.IsOpen())
	{
		// Update
		window.Update();
		panel.Update();
		panel.Draw();

		// Do picking when clicking left mouse button
		if (Mouse::IsButtonPressed(Buttons::Left))
		{
			// Create ray and calculate the world ray
			Picking::GetWorldRay(
				&camera,
				Mouse::GetXPercentage(),
				Mouse::GetYPercentage(),
				ray);

			// If ray collides with room, get room pointer (else nullptr)
			Room *room = boat.GetPickedRoom(ray);

			// Print console info
			std::cout << "-----------------------------------------"
				<< std::endl << std::endl;

			std::cout << "Mouse; x: " << Mouse::GetXPercentage()
				<< ", y: " << Mouse::GetYPercentage() << std::endl;

			std::cout << "Ray; Origin: ("
				<< DirectX::XMVectorGetX(ray.origin) << ", "
				<< DirectX::XMVectorGetY(ray.origin) << ", "
				<< DirectX::XMVectorGetZ(ray.origin) << ")"
				<< ", Direction: ("
				<< DirectX::XMVectorGetX(ray.direction) << ", "
				<< DirectX::XMVectorGetY(ray.direction) << ", "
				<< DirectX::XMVectorGetZ(ray.direction) << ")"
				<< std::endl << std::endl;

			if (room != nullptr)
				std::cout << "HIT ROOM: " << room->GetName() << std::endl << std::endl;
			else
				std::cout << "NO HIT" << std::endl << std::endl;
		}
	}
	return true;
}
