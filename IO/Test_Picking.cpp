#include "Test_Picking.h"

bool TestPickingOnBoat()
{
	//! ADD YOUR OWN TESTS HERE
	Boat boat;
	boat.ReadFile("../../Savefiles/data.boat");

	Window window(L"HELLO", 1920, 1080);

	Panel3D panel(1920, 1080, 0, 0, window.GetWindow(), L"HELLO");

	// Load meshes into list
	Mesh meshList[6] = {
		Mesh("../../Models/Bounding/Bound01.obj"),
		Mesh("../../Models/Bounding/Bound1.obj"),
		Mesh("../../Models/Bounding/Bound2.obj"),
		Mesh("../../Models/DeckMesh/Floor01.obj"),
		Mesh("../../Models/DeckMesh/Floor1.obj"),
		Mesh("../../Models/DeckMesh/Floor2.obj")
	};
	
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

	panel.rGetMeshObject("Bound01")->Translate(0.0f, 0.0f, 3.5f);
	panel.rGetMeshObject("Floor01")->Translate(0.0f, 0.0f, 3.5f);
	panel.rGetMeshObject("Bound2")->Translate(0.0f, 0.0f, -3.5f);
	panel.rGetMeshObject("Floor2")->Translate(0.0f, 0.0f, -3.5f);
	//tempMeshObject->rGetModelMatrix

	DirectX::XMMATRIX *floorMatrixList[3] = {
		panel.rGetMeshObject("Floor01")->rGetModelMatrix(),
		panel.rGetMeshObject("Floor1")->rGetModelMatrix(),
		panel.rGetMeshObject("Floor2")->rGetModelMatrix()
	};

	// Loads the first three meshes from the list and makes bounding boxes
	boat.LoadBoundingBoxes(meshList, floorMatrixList, 3);

	panel.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera(
		0.0f, 0.0f, 0.0f,
		0.f, 1.f, 0.f,
		0.0f, 1.0f, 0.0f,
		90.f, 1.33f,
		//19.2f, 10.8f,
		0.1f, 1000.f/*,
		LOOK_MODE::LOOK_AT,
		PROJECTION_MODE::ORTHOGRAPHIC*/);

	panel.SetCamera(&camera);

	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		panel.Update();
		panel.Draw();
		//if (Mouse::IsButtonPressed(Buttons::Left))
		//{
			Ray ray;
			//std::cout << "x: " << Mouse::GetXPercentage() << ", y: " << Mouse::GetYPercentage();
			Picking::GetWorldRay(&camera, Mouse::GetXPercentage(), Mouse::GetYPercentage(), ray);
			//std::cout << ", Direction (x, y, z): ("
			//	<< DirectX::XMVectorGetX(ray.direction) << ", "
			//	<< DirectX::XMVectorGetY(ray.direction) << ", "
			//	<< DirectX::XMVectorGetZ(ray.direction) << ")"
			//	<< std::endl;
			Room *room = boat.GetPickedRoom(ray);
			if (room != nullptr)
				std::cout << "HIT ROOM: " << room->GetName() << std::endl;
			else
				std::cout << std::endl;
		//}
	}
	return true;
}
