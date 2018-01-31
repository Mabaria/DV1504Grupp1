// BIS.cpp : Defines the entry point for the console application.
//

//#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
//#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
//#include "../../GraphicsEngine/Parsing/Mesh.h"
//#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include <iostream>
#include "Picking.h"
#include "../../GraphicsEngine/Parsing/Mesh.h"
int main()
{
	std::vector<AABB> boxList;
	FillAABBVectorFromFile("../../Models/Bounding/bound.dat", boxList);
	Ray testRay = {
		{ 2.89, 0.03, 0.2, 0.0 },
		DirectX::XMVector3Normalize({0.0f, -1.0f, 0.0f, 1.0f})
	};
	for (int i = 0; i < boxList.size(); i++)
	{
		if (Picking::IsRayIntersectingAABB(testRay, boxList[i]))
		{
			std::cout << "HIT at " << i << std::endl;
		}
	}

	std::cin.ignore();
    return 0;
}


