#include "Actions.h"
#include <iostream>
#include <DDSTextureLoader.h>

Actions::Actions()
{
	this->pDirect3D = nullptr;
	this->mpVertexShader = nullptr;
	this->mpGeometryShader = nullptr;
	this->mpPixelShader = nullptr;
	this->mpInputLayout = nullptr;
	this->mVertexArrayUpdated = false;
}

Actions::~Actions()
{
	if (this->mpVertexShader)
	{
		this->mpVertexShader->Release();
		this->mpVertexShader = nullptr;
	}
	if (this->mpGeometryShader)
	{
		this->mpGeometryShader->Release();
		this->mpGeometryShader = nullptr;
	}
	if (this->mpPixelShader)
	{
		this->mpPixelShader->Release();
		this->mpPixelShader = nullptr;
	}
	if (this->mpInputLayout)
	{
		this->mpInputLayout->Release();
		this->mpInputLayout = nullptr;
	}
	if (this->mpTextureResource)
	{
		this->mpTextureResource->Release();
		this->mpTextureResource = nullptr;
	}
	if (this->mpSamplerState)
	{
		this->mpSamplerState->Release();
		this->mpSamplerState = nullptr;
	}
	if (this->mpVertexBuffer)
	{
		this->mpVertexBuffer->Release();
		this->mpVertexBuffer = nullptr;
	}
	if (this->mpGSBuffer)
	{
		this->mpGSBuffer->Release();
		this->mpGSBuffer = nullptr;
	}
	if (this->mpVertexArray)
	{
		delete[] this->mpVertexArray;
		this->mpVertexArray = nullptr;
	}
}

void Actions::Init(D3D11 *pDirect3D)
{
	this->pDirect3D = pDirect3D;

	//Todo: FAIL HANDLING
	this->CompileShadersAndLayout();
	this->CreateResourceAndSampler();
	this->CreateVertexBuffer();
	this->AddAction(-0.5f, 0.3f, 1);
	this->AddAction(0.0f, -0.7f, 0);
	this->AddAction(0.4f, 0.5f, 3);
}

void Actions::SetCamera(Camera *pCamera)
{
	this->pCamera = pCamera;
}

bool Actions::AddAction(float x, float y, uint32_t data)
{
	if (this->mVertexSize + 1 >= this->mcMaxEvents)
		return false;

	this->mpVertexArray[this->mVertexSize++] = {
		x, 0.0f, y, data
	};

	this->mVertexArrayUpdated = true;
	return true;
}

void Actions::Draw()
{
	if (this->mVertexArrayUpdated)
	{
		this->UpdateVertexBuffer();
		this->mVertexArrayUpdated = false;
	}
	if (this->pCamera != nullptr)
	{
		this->UpdateGSBuffer();
	}
	UINT stride = sizeof(ActionVertex);
	UINT offset = 0;

	this->pDirect3D->GetContext()->IASetInputLayout(this->mpInputLayout);
	this->pDirect3D->GetContext()->VSSetShader(
		this->mpVertexShader,
		nullptr,
		0);
	this->pDirect3D->GetContext()->GSSetShader(
		this->mpGeometryShader,
		nullptr,
		0);
	this->pDirect3D->GetContext()->GSSetConstantBuffers(
		0, 1, &this->mpGSBuffer
	);

	this->pDirect3D->GetContext()->PSSetShader(
		this->mpPixelShader,
		nullptr,
		0);

	this->pDirect3D->GetContext()->PSSetShaderResources(
		0, 1, &this->mpTextureResource);

	//this->pDirect3D->GetContext()->PSSetSamplers(0, 1, &this->mpSamplerState);

	this->pDirect3D->GetContext()->IASetVertexBuffers(
		0,
		1,
		&this->mpVertexBuffer,
		&stride,
		&offset);
	this->pDirect3D->GetContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pDirect3D->GetContext()->Draw(this->mVertexSize, 0);
}

bool Actions::CompileShadersAndLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[2] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"DATA",
			0,
			DXGI_FORMAT_R32_UINT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	return this->pDirect3D->CreateShaders(
		L"../../GraphicsEngine/Shaders/Action_VertexShader.hlsl",
		L"../../GraphicsEngine/Shaders/Action_GeometryShader.hlsl",
		L"../../GraphicsEngine/Shaders/Action_PixelShader.hlsl",
		&this->mpVertexShader,
		&this->mpGeometryShader,
		&this->mpPixelShader,
		inputDesc,
		sizeof(inputDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC),
		&this->mpInputLayout);
}

bool Actions::CreateResourceAndSampler()
{
	if (FAILED(CreateDDSTextureFromFile(
		this->pDirect3D->GetDevice(),
		L"../../Models/Symbols.dds",
		nullptr,
		&this->mpTextureResource)))
	{
		std::cout << "Failed loading image!" << std::endl;
		return false;
	}

	//D3D11_SAMPLER_DESC sampler_desc{};
	//sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	//if (FAILED(this->pDirect3D->GetDevice()->CreateSamplerState(
	//	&sampler_desc,
	//	&this->mpSamplerState)))
	//{
	//	std::cout << "Failed making sampler state!" << std::endl;
	//	return false;
	//}

	//XMFLOAT4 floatData;
	//XMVECTOR cameraPos = pCamera->GetPosition();
	//DirectX::XMStoreFloat4(&floatData, cameraPos);
	//std::cout << sizeof(XMFLOAT4) << std::endl;

	float tempData[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4) * 4;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &tempData;

	if (FAILED(this->pDirect3D->GetDevice()->CreateBuffer(
		&desc,
		&data,
		&this->mpGSBuffer)))
	{
		std::cout << "Failed making GSBuffer" << std::endl;
		return false;
	}

	return true;
}

bool Actions::CreateVertexBuffer()
{
	this->mpVertexArray = new ActionVertex[this->mcMaxEvents];
	memset(this->mpVertexArray, 0, sizeof(ActionVertex)*this->mcMaxEvents);
	this->mVertexSize = 0;

	D3D11_BUFFER_DESC vertex_buffer_desc{};
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	vertex_buffer_desc.ByteWidth = (UINT)(sizeof(ActionVertex) * this->mcMaxEvents);
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{};
	vertex_buffer_data.pSysMem = this->mpVertexArray;

	if (FAILED(this->pDirect3D->GetDevice()->CreateBuffer(
		&vertex_buffer_desc,
		&vertex_buffer_data,
		&this->mpVertexBuffer)))
	{
		MessageBoxA(NULL, "Vertex buffer creation failed.", NULL, MB_OK);
		return false;
	}
	return true;
}

bool Actions::UpdateVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	this->pDirect3D->GetContext()->Map(
		this->mpVertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);

	memcpy(
		mappedResource.pData,
		this->mpVertexArray,
		sizeof(ActionVertex) * this->mcMaxEvents);

	this->pDirect3D->GetContext()->Unmap(this->mpVertexBuffer, 0);

	return true;
}

bool Actions::UpdateGSBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	this->pDirect3D->GetContext()->Map(
		this->mpGSBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);

	XMFLOAT4 floatData;
	XMVECTOR cameraPos = pCamera->GetPosition();
	DirectX::XMStoreFloat4(&floatData, cameraPos);

	memcpy(
		mappedResource.pData,
		&floatData,
		sizeof(XMFLOAT4));

	this->pDirect3D->GetContext()->Unmap(this->mpGSBuffer, 0);

	return true;
}
