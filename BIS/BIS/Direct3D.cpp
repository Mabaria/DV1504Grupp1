#include "stdafx.h"
#include "Direct3D.h"


D3D11::D3D11(
	const HWND& window, 
	const int width, 
	const int height)
{
	this->mWidth = width;
	this->mHeight = height;

	this->mDevice = nullptr;
	this->mContext = nullptr;
	this->mSwapChain = nullptr;
	this->mBackBuffer = nullptr;
	this->mDepthBuffer = nullptr;

	this->mModeDesc = { 0 };


	// -- Sample --
	this->mSample = { 0 };
	this->mSample.Count = 1;
	this->mSample.Quality = 0;

	// -- Viewport --
	this->mViewport.TopLeftX = 0.0f;
	this->mViewport.TopLeftY = 0.0f;
	this->mViewport.Width = (float)this->mWidth;
	this->mViewport.Height = (float)this->mHeight;
	this->mViewport.MinDepth = 0.1f;
	this->mViewport.MaxDepth = 1.0f;


	this->CreateDeviceAndSwapChain(window);
	this->CreateRenderTarget();

	this->mContext->RSSetViewports(1, &this->mViewport);


	this->mClearColor[0] = 0.0f;
	this->mClearColor[1] = 0.0f;
	this->mClearColor[2] = 0.0f;
	this->mClearColor[3] = 1.0f;

	this->ChangeSize(this->mWidth, this->mHeight);
}

D3D11::~D3D11()
{
	this->ReleaseCOM(this->mDevice);
	this->ReleaseCOM(this->mContext);
	this->ReleaseCOM(this->mSwapChain);
	this->ReleaseCOM(this->mBackBuffer);
	this->ReleaseCOM(this->mDepthBuffer);
}

void D3D11::ChangeClearColor(
	const float r,
	const float g,
	const float b)
{
	this->mClearColor[0] = r;
	this->mClearColor[1] = g;
	this->mClearColor[2] = b;
}

void D3D11::ChangeSize(
	const float width,
	const float height)
{
	this->mModeDesc.Width  = this->mWidth  = (UINT)width;
	this->mModeDesc.Height = this->mHeight = (UINT)height;
	this->mSwapChain->ResizeTarget(&this->mModeDesc);
}

const Size& D3D11::GetSize()
{
	this->mSize.width = this->mWidth;
	this->mSize.height = this->mHeight;
	return this->mSize;
}

void D3D11::ReleaseCOM(IUnknown *object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
}

bool D3D11::CreateShaders(
	LPCWSTR vertexShaderPath,
	LPCWSTR geometryShaderPath,
	LPCWSTR pixelShaderPath,
	ID3D11VertexShader **pVertexshader,
	ID3D11GeometryShader **pGeometryShader,
	ID3D11PixelShader **pPixelShader,
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[],
	UINT nrOfElements,
	ID3D11InputLayout **pInputLayout)
{
	// If shaders has no path then it's not possible to create
	if (vertexShaderPath == L"" &&
		pixelShaderPath == L"")
	{
		return false;
	}

	HRESULT hr;

	// Create Vertex Shader
	if (vertexShaderPath != L"")
	{
		ID3DBlob *pVs_blob = nullptr;
		hr = D3DCompileFromFile(
			vertexShaderPath,
			nullptr,
			nullptr,
			"main",
			"vs_5_0",
			0,
			0,
			&pVs_blob,
			nullptr
		);

		hr = this->mDevice->CreateVertexShader(
			pVs_blob->GetBufferPointer(),
			pVs_blob->GetBufferSize(),
			nullptr,
			pVertexshader
		);

		hr = this->mDevice->CreateInputLayout(
			inputElementDesc,
			nrOfElements,
			pVs_blob->GetBufferPointer(),
			pVs_blob->GetBufferSize(),
			pInputLayout
		);

		pVs_blob->Release();
	}

	// Create Geometry Shader
	if (geometryShaderPath != L"")
	{
		ID3DBlob *pGs_blob = nullptr;
		hr = D3DCompileFromFile(
			geometryShaderPath,
			nullptr,
			nullptr,
			"main",
			"gs_5_0",
			0,
			0,
			&pGs_blob,
			nullptr
		);

		hr = this->mDevice->CreateGeometryShader(
			pGs_blob->GetBufferPointer(),
			pGs_blob->GetBufferSize(),
			nullptr,
			pGeometryShader
		);

		pGs_blob->Release();
	}

	// Create Pixel Shader
	if (pixelShaderPath != L"")
	{
		ID3DBlob *pPs_blob = nullptr;
		hr = D3DCompileFromFile(
			pixelShaderPath,
			nullptr,
			nullptr,
			"main",
			"ps_5_0",
			0,
			0,
			&pPs_blob,
			nullptr
		);

		hr = this->mDevice->CreatePixelShader(
			pPs_blob->GetBufferPointer(),
			pPs_blob->GetBufferSize(),
			nullptr,
			pPixelShader
		);

		pPs_blob->Release();
	}

	return true;
}
	// Clear render target and depth buffer 
void D3D11::Clear()
{
	this->mContext->ClearRenderTargetView(this->mBackBuffer, this->mClearColor);
	this->mContext->ClearDepthStencilView(
		this->mDepthBuffer,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}


void D3D11::CreateDeviceAndSwapChain(const HWND& window)
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc	= { 0 };
	swap_chain_desc.BufferCount				= 1;
	swap_chain_desc.BufferDesc.Width		= this->mWidth;
	swap_chain_desc.BufferDesc.Height		= this->mHeight;
	swap_chain_desc.BufferDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage				= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow			= window;
	swap_chain_desc.SampleDesc				= this->mSample;
	swap_chain_desc.Windowed				= TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&this->mSwapChain,
		&this->mDevice,
		NULL,
		&this->mContext
	);
}

void D3D11::CreateRenderTarget()
{
	ID3D11Texture2D *depth_buffer_texture = nullptr;
	ID3D11Texture2D *back_buffer_texture = nullptr;
	this->mSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&back_buffer_texture
	);

	D3D11_TEXTURE2D_DESC depth_buffer_desc = { 0 };
	depth_buffer_desc.Width				= this->mWidth;
	depth_buffer_desc.Height			= this->mHeight;
	depth_buffer_desc.MipLevels			= 1;
	depth_buffer_desc.ArraySize			= 1;
	depth_buffer_desc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc		= this->mSample;
	depth_buffer_desc.Usage				= D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;

	this->mDevice->CreateTexture2D(
		&depth_buffer_desc,
		nullptr,
		&depth_buffer_texture
	);

	this->mDevice->CreateDepthStencilView(
		depth_buffer_texture,
		nullptr,
		&this->mDepthBuffer
	);

	this->mDevice->CreateRenderTargetView(
		back_buffer_texture,
		NULL,
		&this->mBackBuffer
	);

	depth_buffer_texture->Release();
	back_buffer_texture->Release();

	this->mContext->OMSetRenderTargets(
		1, 
		&this->mBackBuffer, 
		this->mDepthBuffer);
}

IDXGISwapChain& D3D11::GetSwapChain() const
{
	return *this->mSwapChain;
}

ID3D11DeviceContext& D3D11::GetContext() const
{
	return *this->mContext;
}

ID3D11Device& D3D11::GetDevice() const
{
	return *this->mDevice;
}
