#include "Test_Camera.h"
#include "Camera.h"
#include "../../UI/UIElements/Panel3D.h"
#include "../../IO/Keyboard.h"
#include <iostream>

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

void TestCameraMovementOrtographic()
{
	Mesh floor2("../../Models/Floor2.obj");
	Mesh floor1("../../Models/Floor1.obj");
	Mesh floor01("../../Models/Floor01.obj");

	MeshObject floor2_object("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors());
	MeshObject floor1_object("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors());
	MeshObject floor01_object("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors());

	std::wstring window_name = L"Demo_BIS";

	Window window(window_name, 1280, 720);

	Panel3D side_view(1280 / 3, 720 / 3, 0, 0, window.GetWindow(), window_name.c_str());
	Panel3D top_view(2 * 1280 / 3, 2 * 720 / 3, 720 / 3, 0, window.GetWindow(), window_name.c_str());

	// Hela båten tjoff in i panelerna och sen mixtras matriserna med.
	side_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"", false);
	side_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"", false);
	side_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"", false);

	top_view.AddMeshObject("floor2", floor2.GetIndexVectors(), floor2.GetVertexVectors(), L"", false);
	top_view.AddMeshObject("floor1", floor1.GetIndexVectors(), floor1.GetVertexVectors(), L"", false);
	top_view.AddMeshObject("floor01", floor01.GetIndexVectors(), floor01.GetVertexVectors(), L"",false );

	float scale = 0.1f;
	top_view.rGetMeshObject("floor2")->Scale(scale, scale, scale);
	top_view.rGetMeshObject("floor1")->Scale(scale, scale, scale);
	top_view.rGetMeshObject("floor01")->Scale(scale, scale, scale);

	top_view.rGetMeshObject("floor2")->Translate(0.0f, 0.0f, 0.5f);
	top_view.rGetMeshObject("floor1")->Translate(0.0f, 0.0f, 0.0f);
	top_view.rGetMeshObject("floor01")->Translate(0.0f, 0.0f, -0.5f);

	side_view.rGetMeshObject("floor2")->Scale(0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor1")->Scale(0.15f, 0.4f, 0.1f);
	side_view.rGetMeshObject("floor01")->Scale(0.15f, 0.4f, 0.1f);

	side_view.rGetMeshObject("floor2")->Translate(0.05f, -0.2f, 0.0f);
	side_view.rGetMeshObject("floor1")->Translate(0.05f, 0.0f, 0.0f);
	side_view.rGetMeshObject("floor01")->Translate(0.05f, 0.2f, 0.0f);

	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	float width		= 2.0f;
	float height	= 2.0f;

	Camera camera2(
		{ 0.0f, 80.0f, -1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, -80.0f, 1.0f, 0.0f },
		width,  height,
		0.01f, 1000.0f, LOOK_TO, ORTHOGRAPHIC);

	Camera camera(
		{ 2.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 15.0f, 16.0f / 9.0f,
		0.1f, 25.0f, LOOK_AT, PERSPECTIVE);

	side_view.SetCamera(&camera);
	top_view.SetCamera(&camera2);

	float speed = 0.1f;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();

		if (Keyboard::IsKeyDown(Keys::W))
		{
			camera2.MoveCamera(0.0f, 0.0f, 1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::S))
		{
			camera2.MoveCamera(0.0f, 0.0f, -1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::D))
		{
			camera2.MoveCamera(1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::A))
		{
			camera2.MoveCamera(-1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Space))
		{
			width += speed;
			height += speed;
			camera2.SetViewWidth(width);
			camera2.SetViewHeight(height);
		}
		else if (Keyboard::IsKeyDown(Keys::Shift))
		{
			width -= speed;
			height -= speed;
			camera2.SetViewWidth(width);
			camera2.SetViewHeight(height);
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

