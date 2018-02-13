#include "Test_Picking.h"
#include <iostream>

void Test_Picking()
{
	//std::vector<AABB> boxList;
	//FillAABBVectorFromFile("../../Models/Bounding/bound.dat", boxList);
	//Ray testRay = {
	//	{ 2.89f, 0.03f, 0.2f, 0.0f },
	//	DirectX::XMVector3Normalize({ 0.0f, -1.0f, 0.0f, 1.0f })
	//};
	//for (unsigned int i = 0; i < boxList.size(); i++)
	//{
	//	if (Picking::IsRayIntersectingAABB(testRay, boxList[i]))
	//	{
	//		std::cout << "HIT at " << i << std::endl;
	//	}
	//}
	//std::cin.ignore();
	// Create camera for testing

	std::cout << "--------- Test picking ---------" << std::endl;
	std::cout << "\nPerspective camera picking:" << std::endl;
	Test_Picking_ClickToRay_Perspective();
	std::cout << "Perspective picking done!" << std::endl;
	std::cout << "\nOrthographic camera picking:" << std::endl;
	Test_Picking_ClickToRay_Orthographic();
	std::cout << "Orthographic picking done!" << std::endl;
	std::cout << "\n--------- Picking done ---------" << std::endl; 
}

void Test_Picking_ClickToRay_Perspective()
{
	std::string printPrefix = "  * ";

	float width, height, nearZ, farZ, fov, aspect;

	width = 100;
	height = 100;
	nearZ = 50;
	farZ = 500;
	fov = 2 * std::atanf(width / (2 * nearZ)) * 180 / PI;
	aspect = width / height;

	std::cout << printPrefix << "Build camera...";

	Camera *pCamera = new Camera(
		2.f, 2.f, 2.f,	// pos
		0.f, 1.f, 0.f,	// Up
		0.f, 0.f, 1.f,	// Look
		fov,
		aspect,
		nearZ,
		farZ,
		LOOK_TO,
		PERSPECTIVE
	);

	std::cout << "done!" << std::endl;

	/**
	*	Test ray to world
	*/

	Ray ray;					// Ray to send into function
	float posX, posY; // Pick coord on window, normalized [0;1]
	float dotP;


	/**
	*	Test 1: Pick center point
	*/

	std::cout << printPrefix << "Pick center point...";

	posX = 0.5f;
	posY = 0.5f;

	// Get ray
	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	dotP = DotProduct(ray.direction, pCamera->GetLookVector());
	
	// Ray direction should be the same as the cameras look direction
	if (dotP != 1.0)
		throw ("Error wrong ray direction from picking (0.5, 0.5) (Perspective)");

	std::cout << "done!" << std::endl;



	/**
	*	Test 2: Pick up left corner
	*/

	std::cout << printPrefix << "Pick up left corner...";

	DirectX::XMVECTOR compare;

	posX = 0.f;
	posY = 0.f;

	// Get ray
	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	// Compute compare vector
	compare = DirectX::XMVector3Normalize({ -1.0f, 1.0f, 1.0f });

	dotP = DotProduct(ray.direction, compare);
	if (dotP < 0.99)
		throw ("Error wrong ray direction from picking (0.0, 0.0) (Perspective)");

	std::cout << "done!" << std::endl;



	/**
	*	Test 3: Pick up right corner
	*/

	std::cout << printPrefix << "Pick up right corner...";

	posX = 1.f;
	posY = 0.f;

	// Get ray
	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	// Compute compare vector
	compare = DirectX::XMVector3Normalize({ 1.0f, 1.0f, 1.0f });

	dotP = DotProduct(ray.direction, compare);
	if (dotP < 0.99)
		throw ("Error wrong ray direction from picking (1.0, 0.0) (Perspective)");

	std::cout << "done!" << std::endl;



	/**
	*	Test 4: Pick down left corner
	*/

	std::cout << printPrefix << "Pick down left corner...";

	posX = 0.f;
	posY = 1.f;

	// Get ray
	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	// Compute compare vector
	compare = DirectX::XMVector3Normalize({ -1.0f, -1.0f, 1.0f });

	dotP = DotProduct(ray.direction, compare);
	if (dotP < 0.99)
		throw ("Error wrong ray direction from picking (0.0, 1.0) (Perspective)");

	std::cout << "done!" << std::endl;



	/**
	*	Test 5: Pick down right corner
	*/

	std::cout << printPrefix << "Pick down right corner...";

	posX = 1.f;
	posY = 1.f;

	// Get ray
	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	// Compute compare vector
	compare = DirectX::XMVector3Normalize({ 1.0f, -1.0f, 1.0f });

	dotP = DotProduct(ray.direction, compare);
	if (dotP < 0.99)
		throw ("Error wrong ray direction from picking (1.0, 1.0) (Perspective)");

	std::cout << "done!" << std::endl;


	delete pCamera;
	
}

void Test_Picking_ClickToRay_Orthographic()
{
	std::string printPrefix = "  * ";

	float width, height, nearZ, farZ;

	width = 100;
	height = 100;
	nearZ = 0;
	farZ = 500;

	std::cout << printPrefix << "Build camera...";

	Camera *pCamera = new Camera(
		2.f, 2.f, 2.f,	// pos
		0.f, 1.f, 0.f,	// Up
		0.f, 0.f, 1.f,	// Look
		width,
		height,
		nearZ,
		farZ,
		LOOK_TO,
		ORTHOGRAPHIC
	);

	std::cout << "done!" << std::endl;

	float posX, posY; // Point of click [0.0;1.0]
	DirectX::XMVECTOR checkPosition, checkDirection;
	Ray ray;
	
	// Direction should always be the same as the camera look vector
	checkDirection = pCamera->GetLookVector();

	/**
	*	Test 1: Pick center point
	*/
	std::cout << printPrefix << "Pick center point...";
	posX = posY = 0.5;
	checkPosition = ComputePosition(pCamera, posX, posY);

	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	if (!CheckRay_Orthographic(ray, checkPosition, checkDirection))
		throw ("Error wrong ray picking (0.5, 0.5) (Orthographic)");

	std::cout << "done!" << std::endl;

	/**
	*	Test 2: Pick up left corner
	*/
	std::cout << printPrefix << "Pick up left corner...";
	posX = 0.0;
	posY = 0.0;
	checkPosition = ComputePosition(pCamera, posX, posY);

	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	if (!CheckRay_Orthographic(ray, checkPosition, checkDirection))
		throw ("Error wrong ray picking (0.0, 0.0) (Orthographic)");

	std::cout << "done!" << std::endl;

	/**
	*	Test 3: Pick up right corner
	*/
	std::cout << printPrefix << "Pick up right corner...";
	posX = 1.0;
	posY = 0.0;
	checkPosition = ComputePosition(pCamera, posX, posY);

	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	if (!CheckRay_Orthographic(ray, checkPosition, checkDirection))
		throw ("Error wrong ray picking (1.0, 0.0) (Orthographic)");

	std::cout << "done!" << std::endl;

	/**
	*	Test 4: Pick down left corner
	*/
	std::cout << printPrefix << "Pick down left corner...";
	posX = 0.0;
	posY = 1.0;
	checkPosition = ComputePosition(pCamera, posX, posY);

	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	if (!CheckRay_Orthographic(ray, checkPosition, checkDirection))
		throw ("Error wrong ray picking (0.0, 1.0) (Orthographic)");

	std::cout << "done!" << std::endl;

	/**
	*	Test 5: Pick down right corner
	*/
	std::cout << printPrefix << "Pick down right corner...";
	posX = 1.0;
	posY = 1.0;
	checkPosition = ComputePosition(pCamera, posX, posY);

	Picking::GetWorldRay(
		pCamera,
		posX,
		posY,
		ray);

	if (!CheckRay_Orthographic(ray, checkPosition, checkDirection))
		throw ("Error wrong ray picking (1.0, 1.0) (Orthographic)");

	std::cout << "done!" << std::endl;

	delete pCamera;
}

float DotProduct(DirectX::XMVECTOR vec1, DirectX::XMVECTOR vec2) 
{
	DirectX::XMFLOAT4 result;

	// Result stored in each component of projection
	vec1 = DirectX::XMVector3Dot(vec1, vec2);
	DirectX::XMStoreFloat4(&result, vec1);

	return result.x;
}

bool CheckRay_Orthographic(
	Ray ray,
	DirectX::XMVECTOR pos,
	DirectX::XMVECTOR dir)
{
	// Result is stored in all components of the returning vector
	DirectX::XMVECTOR result;

	result = DirectX::XMVectorEqual(ray.origin, pos);
	if (DirectX::XMVectorGetX(result) == 0)
		return false; // Position is wrong

	result = DirectX::XMVectorEqual(ray.direction, dir);
	if (DirectX::XMVectorGetX(result) == 0)
		return false; // Direction is wrong

	return true;
}

DirectX::XMVECTOR ComputePosition(Camera *pCamera, float posX, float posY)
{
	float stepX = posX * 2.f - 1.f;
	float stepY = (posY * 2.f - 1.f) * (-1.f);

	float width = pCamera->GetViewWidth();
	float height = pCamera->GetViewHeight();

	DirectX::XMVECTOR upVec = pCamera->GetUpVector();
	DirectX::XMVECTOR frontVec = pCamera->GetLookVector();
	DirectX::XMVECTOR rightVec = DirectX::XMVector3Cross(
		pCamera->GetUpVector(),
		pCamera->GetLookVector());

	upVec = DirectX::XMVectorScale(
		upVec,
		stepY * (height/2));

	frontVec = DirectX::XMVectorScale(
		frontVec,
		pCamera->GetNearZ());
	
	rightVec = DirectX::XMVectorScale(
		rightVec,
		stepX * (width/2));
	
	DirectX::XMVECTOR position = pCamera->GetPosition();
	position = DirectX::XMVectorAdd(
		position,
		frontVec);
	position = DirectX::XMVectorAdd(
		position,
		upVec);
	position = DirectX::XMVectorAdd(
		position,
		rightVec);

	return position;
}

bool TestPickingOnBoat()
{
	//! ADD YOUR OWN TESTS HERE
	Boat boat;
	boat.ReadFile("../../Savefiles/Testboat1.boat");
	//x boat.LoadBoatMesh();
	//x boat.LoadBoundingBoxes();

	Window window(L"HELLO", 1920, 1080);

	Panel3D panel(1920, 1080, 0, 0, window.GetWindow(), L"HELLO");

	Mesh meshList[6] = {
		Mesh("../../Models/Bounding/Bound01.obj"),
		Mesh("../../Models/Bounding/Bound1.obj"),
		Mesh("../../Models/Bounding/Bound2.obj"),
		Mesh("../../Models/DeckMesh/Floor01.obj"),
		Mesh("../../Models/DeckMesh/Floor1.obj"),
		Mesh("../../Models/DeckMesh/Floor2.obj")
	};

	MeshObject meshObjectList[6] = {
		MeshObject(
			"Bound01",
			meshList[0].GetIndexVectors(),
			meshList[0].GetVertexVectors()
		),
		MeshObject(
			"Bound1",
			meshList[1].GetIndexVectors(),
			meshList[1].GetVertexVectors()
		),
		MeshObject(
			"Bound2",
			meshList[2].GetIndexVectors(),
			meshList[2].GetVertexVectors()
		),
		MeshObject(
			"Floor01",
			meshList[3].GetIndexVectors(),
			meshList[3].GetVertexVectors()
		),
		MeshObject(
			"Floor1",
			meshList[4].GetIndexVectors(),
			meshList[4].GetVertexVectors()
		),
		MeshObject(
			"Floor2",
			meshList[5].GetIndexVectors(),
			meshList[5].GetVertexVectors()
		)
	};

	panel.AddMeshObject(&meshObjectList[0]);
	panel.AddMeshObject(&meshObjectList[1]);
	panel.AddMeshObject(&meshObjectList[2]);
	panel.AddMeshObject(&meshObjectList[3]);
	panel.AddMeshObject(&meshObjectList[4]);
	panel.AddMeshObject(&meshObjectList[5]);

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
		0.0f, 4.0f, 0.0f,
		0.f, 0.f, 1.f,
		0.0f, 0.0f, 0.0f,
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
	}
	return true;
}
