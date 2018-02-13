#include "Test_MaterialHandler.h"

#include "MaterialHandler.h"
#include "../../UI/UIElements/MeshObject.h"
#include "../Window.h"
#include "../../UI/UIElements/Panel3D.h"
#include "../../IO/Keyboard.h"
#include "../../IO/Mouse.h"

void Test_Material_Handler()
{

	Mesh material_test_boat("../../Models/Floor2Materials.obj");
	Mesh mixbag("../../Models/MixbagAttached.3ds");

	MeshObject mat_test_boat_object("Cube", &material_test_boat);
	MeshObject mixbag_object("Mixbag", &mixbag);
	mixbag_object.Scale(0.2f, 0.2f, 0.2f);
	mixbag_object.Translate(-30.0f, -10.0f, 0.0f);

	mat_test_boat_object.Scale(50.5, 50.5, 50.5);
	//mat_test_boat_object.Rotate(0.0f, 0.00f, 3.14f);

	mat_test_boat_object.pGetMaterialHandler()->GetDiffuseColor(0);

	mat_test_boat_object.pGetMaterialHandler()->GetSpecularColor(
	material_test_boat.GetSubmeshMaterialIndex(0));

	mat_test_boat_object.pGetMaterialHandler()->GetAmbientColor(
		material_test_boat.GetSubmeshMaterialIndex(0));

	MaterialStruct testMaterial =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f,
		1.0f
	};

	mat_test_boat_object.pGetMaterialHandler()->SetMaterialStruct(testMaterial, 0);
	mat_test_boat_object.pGetMaterialHandler()->SetMaterialStruct(testMaterial, 2);

	std::wstring window_name = L"Demo_BIS";

	Window window(window_name, 1280, 720);

	Panel3D side_view(1280 / 3, 720 / 3, 0, 0, window.GetWindow(), window_name.c_str());
	Panel3D top_view(2 * 1280 / 3, 2 * 720 / 3, 720 / 3, 0, window.GetWindow(), window_name.c_str());

	top_view.AddMeshObject(&mat_test_boat_object);
	side_view.AddMeshObject(&mixbag_object);


	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera2(
		{ 0.0f, 20.0f, -20.2f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		90.0f, 1280/720,
		0.001f, 100.0f, LOOK_AT, PERSPECTIVE);

	Camera camera(
		{ 0.0f, 100.0f, 0.00001f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		192 * 5, 108 * 5,
		0.000000000001f, 1000.0f, LOOK_AT, ORTHOGRAPHIC);

	side_view.SetCamera(&camera2);
	top_view.SetCamera(&camera);

	float speed = 0.1f;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();
	//	top_view.rGetMeshObject("Cube")->Rotate(0.0f, 0.01f, 0.0f);

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
			camera2.MoveCamera(0.0f, 1.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Shift))
		{
			camera2.MoveCamera(0.0f, -1.0f, 0.0f, speed);
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