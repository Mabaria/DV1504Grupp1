#include "Test_Picking.h"
#include "Picking.h"
#include <iostream>

void Test_Picking()
{
	std::vector<AABB> boxList;
	FillAABBVectorFromFile("../../Models/Bounding/bound.dat", boxList);
	Ray testRay = {
		{ 2.89f, 0.03f, 0.2f, 0.0f },
		DirectX::XMVector3Normalize({ 0.0f, -1.0f, 0.0f, 1.0f })
	};
	for (unsigned int i = 0; i < boxList.size(); i++)
	{
		if (Picking::IsRayIntersectingAABB(testRay, boxList[i]))
		{
			std::cout << "HIT at " << i << std::endl;
		}
	}

	std::cin.ignore();
}
