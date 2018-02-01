#include "Test_BoatOnScreen.h"
//#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../UI/UIElements/Panel3D.h"
#include "../../GraphicsEngine/Window.h"
#include "../../GraphicsEngine/Camera/Camera.h"
#include "../../IO/Keyboard.h"
#include "../../IO/Mouse.h"

struct mat_struct
{
	XMMATRIX world;
	XMMATRIX vp;
};

void TestBoatOnScreen()
{
	Mesh boatMesh("../../Models/Floor2.obj");
	Window window(L"800", 800, 600);
	Panel3D panel(800, 600, 0, 0, window.GetWindow(), L"800");

	panel.AddMeshObject(
		"boat", 
		boatMesh.GetIndexVectors(), 
		boatMesh.GetVertexVectors());

	ID3D11VertexShader* pVS = nullptr;
	ID3D11GeometryShader* pGS = nullptr;
	ID3D11PixelShader* pPS = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;

	const UINT n_inputs = 3;
	D3D11_INPUT_ELEMENT_DESC input_desc[n_inputs] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	panel.CreateShadersAndSetup(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl",
		&pVS,
		&pGS,
		&pPS,
		input_desc,
		n_inputs,
		&pInputLayout);


	ID3D11Buffer* matrix_buffer = nullptr;
	D3D11_BUFFER_DESC matrix_buffer_desc{};
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.ByteWidth = sizeof(mat_struct);
	matrix_buffer_desc.StructureByteStride = 0;

	XMMATRIX perspective = 
		//XMMatrixOrthographicLH(2, 2, 0.1f, 100.0f);
		XMMatrixPerspectiveFovLH(XM_PI / 8.0f, 8.0f / 6.0f, 0.1f, 1000.0f);

	Camera camera({ 2.0f, 5.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.f, 0.0f });
	//Camera camera({ 0.0f, 5.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.00000001f, -1.0f, 0.0f, 0.0f }, CAMERAMODE::LOOKTO);

	

	mat_struct mat;

	mat.world = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	mat.world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	mat.vp = camera.GetViewMatrix() * perspective;

	D3D11_SUBRESOURCE_DATA matrix_data{};
	matrix_data.pSysMem = &mat;


	panel.rGetDirect3D().GetDevice()->CreateBuffer(&matrix_buffer_desc, &matrix_data, &matrix_buffer);

	panel.rGetDirect3D().GetContext()->VSSetConstantBuffers(0, 1, &matrix_buffer);

	D3D11_MAPPED_SUBRESOURCE map;

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

		mat.world = XMMatrixScaling(0.1f, 0.1f, 0.1f);
		mat.world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		mat.vp = camera.GetViewMatrix() * perspective;

		matrix_data.pSysMem = &mat;

		panel.rGetDirect3D().GetContext()->Map(matrix_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &map);
		memcpy(map.pData, &mat, sizeof(mat));
		panel.rGetDirect3D().GetContext()->Unmap(matrix_buffer, NULL);


		panel.Draw();
	}
	pVS->Release();
	pPS->Release();
	pInputLayout->Release();
}
