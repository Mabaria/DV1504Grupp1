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

	

	Camera *pCam = new Camera(
		2.f, 2.f, 2.f,	// pos
		0.f, 1.f, 0.f,	// Up
		1.f, 0.f, 0.f,	// Look
		800,						// Width
		500,						// Height
		50,							// Near
		200,						// Far
		LOOK_TO,
		PERSPECTIVE
	);

	/**
	*	Test ray to world
	*/

	Ray ray;
	float posX, posY; // Pick coord on window, normalized [0;1]

	// Center of screen
	posX = 0.5f;
	posY = 0.5f;

	// Get ray
	Picking::GetWorldRay(
		pCam->GetProjectionMatrix(),
		pCam->GetViewMatrix(),
		posX,
		posY,
		ray);

	
		


	delete pCam;

}
