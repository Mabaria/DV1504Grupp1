#include "../GraphicsEngine/Test_Graphics.h"

#include <DDSTextureLoader.h>

void Test_Window()
{
	Window window(L"Test_Window", 1280, 720);

	window.Open();

	// To see that the value is stored correctly
	int width = window.GetClientSize().width;
	HWND hwnd = window.GetWindow();

	while (window.IsOpen())
	{
		window.Update();

		// Print size
		std::cout << window.GetClientSize().width << std::endl;
		std::cout << window.GetClientSize().height << std::endl;
		// The last 2 sizes will be null values since the window is destroyed
	}


	std::cout << width << std::endl;

	system("pause");

}

void Test_D3D11()
{
	Window window(L"Test_D3D11", 1280, 720);
	
	int x = 500;
	int y = 500;
	float width = 0.5f;
	float height = 0.5f;

	HWND child_window = CreateWindowEx(
		0,
		L"Test_D3D11",
		L"Child",
		WS_CHILD | WS_BORDER,
		0,
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		window.GetWindow(),
		0,
		GetModuleHandle(0),
		0
	);

	D3D11 direct3D(
		/*child_window,*/
		window.GetClientSize().width * width,
		window.GetClientSize().height * height
	);
	direct3D.Init(child_window);

	ID3D11InputLayout *pInputLayout = nullptr;
	ID3D11VertexShader *pVS = nullptr;
	ID3D11PixelShader *pPS = nullptr;
	D3D11_INPUT_ELEMENT_DESC input_desc = { 
		"POSITION", 
		0, 
		DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 
		0, 
		D3D11_INPUT_PER_VERTEX_DATA
	};

	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 0.0f,
		 0.0f,  0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.5f, 0.0f,
	};

	D3D11_BUFFER_DESC vertex_buffer_desc = { 0 };
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.ByteWidth = sizeof(float) * 3 * 4;
	
	D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
	vertex_data.pSysMem = vertices;

	ID3D11Buffer *vertex_buffer = nullptr;

	direct3D.GetDevice()->CreateBuffer(
		&vertex_buffer_desc,
		&vertex_data,
		&vertex_buffer
	);

	direct3D.CreateShaders(
		L"../../GraphicsEngine/Test_VertexShader.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader.hlsl",
		&pVS,
		nullptr,
		&pPS,
		&input_desc,
		(UINT)1,
		&pInputLayout
	);

	direct3D.GetContext()->VSSetShader(pVS, nullptr, 0);
	direct3D.GetContext()->GSSetShader(nullptr, nullptr, 0);
	direct3D.GetContext()->PSSetShader(pPS, nullptr, 0);

	UINT stride = 4 * sizeof(float);
	UINT offset = 0;
	direct3D.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	direct3D.GetContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	direct3D.GetContext()->IASetInputLayout(pInputLayout);

	window.Open();
	ShowWindow(child_window, SW_NORMAL);

	while (window.IsOpen())
	{
		if (!window.Update())
		{
			direct3D.Clear();
			direct3D.GetContext()->Draw(3, 0);
			direct3D.GetSwapChain()->Present(1, 0);

			direct3D.ChangeSize(
				window.GetClientSize().width * width,
				window.GetClientSize().height * height
			);
		}
	}

	pVS->Release();
	pPS->Release();
	pInputLayout->Release();
	vertex_buffer->Release();
}

void Test_Text3D()	// !!! NOTE: Render transparent object last !!!
{

	Window window(L"Test_Text3D", 1280, 720);

	D3D11 direct3D(
		window.GetWindow(),
		window.GetClientSize().width,
		window.GetClientSize().height
	);


	ID3D11SamplerState *ss = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;
	HRESULT hr = DirectX::CreateDDSTextureFromFile(
		direct3D.GetDevice(),
		L"Font3D.DDS",
		nullptr,
		&srv
	);
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	direct3D.GetDevice()->CreateSamplerState(&sampler_desc, &ss);


	ID3D11InputLayout *pInputLayout = nullptr;
	ID3D11VertexShader *pVS = nullptr;
	ID3D11PixelShader *pPS = nullptr;
	D3D11_INPUT_ELEMENT_DESC input_desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA },
	};

	float vertices[] = {
		// Vertex Position				// TexCoord
		 -0.5f, -0.5f,	0.5f, 1.0f,		0.0f, 1.0f,	// Left down
		 -0.5f,  0.5f,	0.5f, 1.0f,		0.0f, 0.0f,	// Left top
		  0.5f, -0.5f,	0.5f, 1.0f,		1.0f, 1.0f,	// Right down
		  0.5f,  0.5f,	0.5f, 1.0f,		1.0f, 0.0f,	// Right top
	};

	D3D11_BUFFER_DESC vertex_buffer_desc = { 0 };
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.ByteWidth = sizeof(float) * 6 * 4;

	D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
	vertex_data.pSysMem = vertices;

	ID3D11Buffer *vertex_buffer = nullptr;
	ID3D11Buffer *vertex_buffer2 = nullptr;

	direct3D.GetDevice()->CreateBuffer(
		&vertex_buffer_desc,
		&vertex_data,
		&vertex_buffer
	);

	// --- Test ---

	ID3D11ShaderResourceView *srv2 = nullptr;
	hr = DirectX::CreateDDSTextureFromFile(
		direct3D.GetDevice(),
		L"Test.DDS",
		nullptr,
		&srv2
	);

	float vertices2[] = {
		// Vertex Position				// TexCoord
		-1.0f, -0.25f,	0.7f, 1.0f,		0.0f, 1.0f,	// Left down
		-1.0f,  0.19f,	0.7f, 1.0f,		0.0f, 0.0f,	// Left top
		 1.0f, -0.25f,	0.7f, 1.0f,		1.0f, 1.0f,	// Right down
		 1.0f,  0.19f,	0.7f, 1.0f,		1.0f, 0.0f,	// Right top
	};

	vertex_data.pSysMem = vertices2;

	direct3D.GetDevice()->CreateBuffer(
		&vertex_buffer_desc,
		&vertex_data,
		&vertex_buffer2
	);

	// --- ---

	direct3D.CreateShaders(
		L"../../GraphicsEngine/Test_VertexShader_Text3D.hlsl",
		L"",
		L"../../GraphicsEngine/Test_PixelShader_Text3D.hlsl",
		&pVS,
		nullptr,
		&pPS,
		input_desc,
		(UINT)2,
		&pInputLayout
	);

	direct3D.GetContext()->VSSetShader(pVS, nullptr, 0);
	direct3D.GetContext()->GSSetShader(nullptr, nullptr, 0);
	direct3D.GetContext()->PSSetShader(pPS, nullptr, 0);

	direct3D.GetContext()->PSSetSamplers(0, 1, &ss);

	UINT stride = 6 * sizeof(float);
	UINT offset = 0;
	direct3D.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	direct3D.GetContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	direct3D.GetContext()->IASetInputLayout(pInputLayout);

	window.Open();

	while (window.IsOpen())
	{
		if (!window.Update())
		{
			direct3D.Clear();

			direct3D.GetContext()->PSSetShaderResources(0, 1, &srv2);
			direct3D.GetContext()->IASetVertexBuffers(0, 1, &vertex_buffer2, &stride, &offset);
			direct3D.GetContext()->Draw(4, 0);

			direct3D.GetContext()->PSSetShaderResources(0, 1, &srv);
			direct3D.GetContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
			direct3D.GetContext()->Draw(4, 0);


			direct3D.GetSwapChain()->Present(1, 0);
		}
	}

	pVS->Release();
	pPS->Release();
	pInputLayout->Release();
	vertex_buffer->Release();
	srv->Release();
	srv2->Release();
	ss->Release();
	vertex_buffer2->Release();
}