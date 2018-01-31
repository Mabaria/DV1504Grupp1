#include "Test_BoatOnScreen.h"
//#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../UI/UIElements/Panel3D.h"
#include "../../GraphicsEngine/Window.h"
void TestBoatOnScreen()
{
	Mesh boatMesh("../../Models/TestBoat.obj");
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
	matrix_buffer_desc.ByteWidth = sizeof(XMMATRIX);
	matrix_buffer_desc.StructureByteStride = 0;

	XMMATRIX matrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	matrix *= XMMatrixRotationRollPitchYaw(-XM_PI/2.0f, 0, 0);
	matrix *= XMMatrixTranslation(0.0f, 0.0f, 0.5f);

	struct mat_struct
	{
		XMMATRIX matrix;
	};

	mat_struct mat{ matrix };

	D3D11_SUBRESOURCE_DATA matrix_data{};
	matrix_data.pSysMem = &matrix;


	panel.rGetDirect3D().GetDevice()->CreateBuffer(&matrix_buffer_desc, &matrix_data, &matrix_buffer);

	panel.rGetDirect3D().GetContext()->VSSetConstantBuffers(0, 1, &matrix_buffer);

	window.Open();
	while (window.IsOpen())
	{
		window.Update();
		panel.Draw();
	}
	pVS->Release();
	pPS->Release();
	pInputLayout->Release();
}
