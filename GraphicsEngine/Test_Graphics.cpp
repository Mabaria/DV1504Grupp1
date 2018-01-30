#include "../GraphicsEngine/Test_Graphics.h"


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
		child_window,
		window.GetClientSize().width * width,
		window.GetClientSize().height * height
	);

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