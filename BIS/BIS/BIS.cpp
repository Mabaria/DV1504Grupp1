// BIS.cpp : Defines the entry point for the console application.
//

#include "../../GraphicsEngine/Parsing/Test_AssImp.h"
#include "../../GraphicsEngine/Parsing/Test_Mesh.h"
#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../GraphicsEngine/Parsing/Test_Boat.h"
#include "Picking.h"
int main()
{
//	TestMesh();
//	TestAssimp();
//	TestBoat();
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookToLH (
		{ 3.0f, 4.0f, -16.0f },
		{ 1.f, 1.f, 1.0f },
		{ 0.0f, 1.0f, 0.0f }
	);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveLH(
		800.0f,
		600.0f,
		1.0f,
		100.0f
	);

	DirectX::XMVECTOR rayOrigin;
	DirectX::XMVECTOR rayDirection;
	Picking::GetWorldRay (
		projectionMatrix,
		viewMatrix,
		1.0f,
		1.0f,
		rayOrigin,
		rayDirection);

    return 0;
}


