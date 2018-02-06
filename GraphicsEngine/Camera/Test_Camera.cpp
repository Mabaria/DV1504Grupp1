#include "Test_Camera.h"
#include "Camera.h"
void TestCamera()
{
	using namespace DirectX;
	Camera test1(0.4f, 12.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		21.0f, 0.0f, 1.0f,
		1600,
		900,
		10.0f,
		1000.0f,
		LOOK_AT,
		ORTHOGRAPHIC);
	Camera test2(DirectX::XMVectorSet(0.4f, 12.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(21.0f, 0.0f, 1.0f, 0.0f),
		90.0f,
		16.0f / 9.0f,
		10.0f,
		1000.0f
	);
	Camera test3(DirectX::XMFLOAT3(0.4f, 12.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(21.0f, 0.0f, 1.0f),
		90.0f,
		16.0f / 9.0f,
		10.0f,
		1000.0f);

	test1.SetCameraPosition(5.0f, 3.0f, -2.0f);
	test2.SetCameraPosition(DirectX::XMVectorSet(5.0f, 3.0f, -2.0f, 0.0f));
	test3.SetCameraPosition(XMFLOAT3(5.0f, 3.0f, -2.0f));

	test1.MoveCamera(1.0f, 0.2f, 3.0f, 15.0f);
	test2.MoveCamera(XMVectorSet(1.0f, 0.2f, 3.0f, 0.0f), 15.0f);
	test3.MoveCamera(XMFLOAT3(1.0f, 0.2f, 3.0f), 15.0f);

	test1.SetUpVector(1.0f, 0.0f, 0.0f);
	test2.SetUpVector(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	test3.SetUpVector(XMFLOAT3(1.0f, 0.0f, 0.0f));

	test1.RotateCameraPitchYawRoll(0.4f, 0.1f, -0.7f);
	test2.RotateCameraPitchYawRoll(DirectX::XMVectorSet(0.4f, 0.1f, -0.7f, 0.0f));
	test3.RotateCameraPitchYawRoll(DirectX::XMFLOAT3(0.4f, 0.1f, -0.7f));

	test1.GetPosition();
	test2.GetUpVector();
	test3.GetLookVector();

	test1.GetViewMatrix();
	test2.GetViewMatrix();
	test3.GetViewMatrix();

	test1.GetTransposedViewMatrix();

}
