#include "Test_MaterialHandler.h"

#include "MaterialHandler.h"
#include "../../UI/UIElements/MeshObject.h"
#include "../Window.h"
#include "../../UI/UIElements/Panel3D.h"
#include "../../IO/Keyboard.h"
#include "../../IO/Mouse.h"

void Test_Material_Handler()
{

	Mesh material_test_cube("../../Models/RainbowTest.3DS");

	MeshObject mat_test_cube_object("Cube", &material_test_cube);

	mat_test_cube_object.Scale(0.05, 0.05, 0.05);

	mat_test_cube_object.pGetMaterialHandler()->GetDiffuseColor(0);
	mat_test_cube_object.pGetMaterialHandler()->GetSpecularColor(
	material_test_cube.GetSubmeshMaterialIndex(0));
	mat_test_cube_object.pGetMaterialHandler()->GetAmbientColor(
		material_test_cube.GetSubmeshMaterialIndex(0));
	std::wstring window_name = L"Demo_BIS";

	Window window(window_name, 1280, 720);

	Panel3D side_view(1280 / 3, 720 / 3, 0, 0, window.GetWindow(), window_name.c_str());
	Panel3D top_view(2 * 1280 / 3, 2 * 720 / 3, 720 / 3, 0, window.GetWindow(), window_name.c_str());

	// Hela b�ten tjoff in i panelerna och sen mixtras matriserna med.

	/*top_view.AddMeshObject(&floor2_object);
	top_view.AddMeshObject(&floor1_object);
	top_view.AddMeshObject(&floor01_object);

	top_view.rGetMeshObject("floor2")->Scale(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor1")->Scale(0.1f, 0.1f, 0.1f);
	top_view.rGetMeshObject("floor01")->Scale(0.1f, 0.1f, 0.1f);

	top_view.rGetMeshObject("floor2")->Translate(0.0f, 0.0f, 0.5f);
	top_view.rGetMeshObject("floor1")->Translate(0.0f, 0.0f, 0.0f);
	top_view.rGetMeshObject("floor01")->Translate(0.0f, 0.0f, -0.5f);*/

	top_view.AddMeshObject(&mat_test_cube_object);


	side_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	top_view.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl");

	Camera camera2(
		{ 0.0f, 0.0f, -0.2f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		2.0f, 2.0f,
		0.1f, 100.0f, LOOK_AT, ORTHOGRAPHIC);

	Camera camera(
		{ 0.0f, 5.0f, 3.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		XM_PI / 8.0f, 16.0f / 9.0f,
		0.1f, 1000.0f, LOOK_AT, PERSPECTIVE);

	side_view.SetCamera(&camera2);
	top_view.SetCamera(&camera);

	float speed = 0.1f;

	window.Open();
	while (window.IsOpen())
	{
		window.Update();

		if (Keyboard::IsKeyDown(Keys::W))
		{
			camera.MoveCamera(0.0f, 0.0f, 1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::S))
		{
			camera.MoveCamera(0.0f, 0.0f, -1.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::D))
		{
			camera.MoveCamera(1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::A))
		{
			camera.MoveCamera(-1.0f, 0.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Space))
		{
			camera.MoveCamera(0.0f, 1.0f, 0.0f, speed);
		}
		else if (Keyboard::IsKeyDown(Keys::Shift))
		{
			camera.MoveCamera(0.0f, -1.0f, 0.0f, speed);
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