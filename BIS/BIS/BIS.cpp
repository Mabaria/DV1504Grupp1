// BIS.cpp : Defines the entry point for the console application.
//

//#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
//#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
//#include "../../GraphicsEngine/Parsing/Mesh.h"
//#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "Picking.h"
#include "../../GraphicsEngine/Parsing/Mesh.h"
int main()
{
//	TestMesh();
//	TestAssimp();
//	TestBoat();
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH
	//(
	//	{ 3.0f, 4.0f, -16.0f },
	//	{ 60.f, 21.f, 2.0f },
	//	{ 0.0f, 1.0f, 0.0f }
	//);

	Mesh bound01("../../Models/Bounding/Bound01.obj");
	AABB test = FromIndexedMeshToAABB(bound01, 0);

    return 0;
}


