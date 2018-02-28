#include "Actions.h"
#include <iostream>
#include <DDSTextureLoader.h>
#include <stdlib.h>

Actions::Actions()
{
	this->pDirect3D = nullptr;
	this->pCamera = nullptr;
	this->pMovableCamera = nullptr;

	this->mpVertexShader = nullptr;
	this->mpGeometryShader = nullptr;
	this->mpPixelShader = nullptr;
	this->mpInputLayout = nullptr;
	this->mpIconResource = nullptr;
	this->mpNumberResource = nullptr;
	this->mpSamplerState = nullptr;
	this->mpGSBuffer = nullptr;

	this->mVertexSize = 0;
	this->mpVertexArray = nullptr;
	this->mpVertexPtrArray = nullptr;
	this->mpVertexBuffer = nullptr;
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
	if (this->mpIconResource)
	{
		this->mpIconResource->Release();
		this->mpIconResource = nullptr;
	}
	if (this->mpNumberResource)
	{
		this->mpNumberResource->Release();
		this->mpNumberResource = nullptr;
	}
	if (this->mpSamplerState)
	{
		this->mpSamplerState->Release();
		this->mpSamplerState = nullptr;
	}
	if (this->mpGSBuffer)
	{
		this->mpGSBuffer->Release();
		this->mpGSBuffer = nullptr;
	}
	if (this->mpVertexBuffer)
	{
		this->mpVertexBuffer->Release();
		this->mpVertexBuffer = nullptr;
	}
	if (this->mpVertexArray)
	{
		delete[] this->mpVertexArray;
		this->mpVertexArray = nullptr;
	}
	if (this->mpVertexPtrArray)
	{
		for (int i = 0; i < this->mVertexSize; i++)
			delete this->mpVertexPtrArray[i];
		delete[] this->mpVertexPtrArray;
		this->mpVertexPtrArray = nullptr;
	}
}

bool Actions::Init(D3D11 *pDirect3D)
{
	// A Direct3D pointer used in the 3D panel is necessary
	this->pDirect3D = pDirect3D;

	if (!this->CompileShadersAndLayout())
	{
		// Add error message?
		return false;
	}
	if (!this->CreateResources())
	{
		// Add error message?
		return false;
	}
	if (!this->CreateVertexBuffer())
	{
		// Add error message?
		return false;
	}

	// ------- TEST TEST TEST TEST TEST -------
	// The following is a test that adds 30 actions.
	// The "ActionPtr" is stored in a temporary list.
	// Using the list, delete the first 25 actions.
	// Something to point out here is that no memory leaks will occur
	// when "tempList" goes out of scope. The Action class takes care
	// of this in its destructor.

	ActionPtr *tempList[30];
	for (int i = 0; i < 30; i++)
	{
		uint32_t hello = Icon_Injured_Reported | Rotation_Stationary;
		tempList[i] = this->AddAction(
			(float)(rand() % 1000) / 500.f - 1.0f,
			(float)(rand() % 1000) / 500.f - 1.0f,
			Icon_Injured_Moved | Rotation_0 | Number_4);
		// Feel free to experiment with your own combination of flags
	}
	for (int i = 0; i < 25; i++)
	{
		this->RemoveAction(&tempList[i]);
	}
	// ------- END OF TEST -------

	return true;
}

void Actions::SetCamera(Camera *pCamera)
{
	this->pCamera = pCamera;
}

void Actions::SetMoveableCamera(MovableCameraComponent *pMovableCamera)
{
	this->pMovableCamera = pMovableCamera;
}

Actions::ActionPtr *Actions::AddAction(float x, float y, uint32_t data)
{
	// Early exit if it would exceed max events
	if (this->mVertexSize + 1 >= this->mcMaxEvents)
		return nullptr;

	// Fill in vertex info from the input parameters
	this->mpVertexArray[this->mVertexSize] = {
		x, 0.0f, y, data
	};
	// Create a new Action Pointer with the right index
	ActionPtr *returnPtr = new ActionPtr(this->mVertexSize);
	// Store the pointer in an array
	this->mpVertexPtrArray[this->mVertexSize++] =
		returnPtr;

	// Tell that the array has been updated and return the pointer
	this->mVertexArrayUpdated = true;
	return returnPtr;
}

void Actions::RemoveAction(ActionPtr **pActionPtr)
{
	// If the action is already removed (nullptr), then early exit
	if ((*pActionPtr) != nullptr)
	{
		// Following code takes the last element of the vertex and pointer
		// array and replaces the requested action. The action pointer will
		// also be deleted so no loose ends/memory leaks remains.

		this->mpVertexArray[(*pActionPtr)->index] =
			this->mpVertexArray[--this->mVertexSize];
		this->mpVertexPtrArray[(*pActionPtr)->index] =
			this->mpVertexPtrArray[this->mVertexSize];
		this->mpVertexPtrArray[(*pActionPtr)->index]->index = (*pActionPtr)->index;
		delete (*pActionPtr);
		(*pActionPtr) = nullptr;
		this->mVertexArrayUpdated = true;
	}
}

void Actions::Draw()
{
	// Upload data from CPU to GPU only if the array is updated
	if (this->mVertexArrayUpdated)
	{
		this->UpdateVertexBuffer();
		this->mVertexArrayUpdated = false;
	}
	// Update data about the camera. Safety-check if, for some reason, the
	// camera haven't been added.
	if (this->pCamera != nullptr)
	{
		this->UpdateGSBuffer();
	}

	// Following code sets all necessary resources before the draw call.
	UINT stride = sizeof(ActionVertex);
	UINT offset = 0;

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
		0, 1, &this->mpIconResource);
	this->pDirect3D->GetContext()->PSSetShaderResources(
		1, 1, &this->mpNumberResource);

	this->pDirect3D->GetContext()->IASetInputLayout(this->mpInputLayout);
	this->pDirect3D->GetContext()->IASetVertexBuffers(
		0,
		1,
		&this->mpVertexBuffer,
		&stride,
		&offset);
	this->pDirect3D->GetContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Draw only the amount of vertecies that are active
	this->pDirect3D->GetContext()->Draw(this->mVertexSize, 0);
}

bool Actions::CompileShadersAndLayout()
{
	// Compiles shaders and input layouts

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

bool Actions::CreateResources()
{
	// Create Shader Resources from files
	if (FAILED(CreateDDSTextureFromFile(
		this->pDirect3D->GetDevice(),
		L"../../Models/Symbols.dds",
		nullptr,
		&this->mpIconResource)))
	{
		std::cout << "Failed loading image!" << std::endl;
		return false;
	}

	if (FAILED(CreateDDSTextureFromFile(
		this->pDirect3D->GetDevice(),
		L"../../Models/Numbers.dds",
		nullptr,
		&this->mpNumberResource)))
	{
		std::cout << "Failed loading image!" << std::endl;
		return false;
	}

	// Just zero data if, for some reason, the camera wouldn't be loaded into
	// this class.
	CameraData tempData = {
		{ 0.0f, 0.0f, 0.0f },
		0 };

	// Create buffer for camera data
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CameraData);
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
	// Initialize vertex data array and vertex pointer array
	this->mpVertexArray = new ActionVertex[this->mcMaxEvents];
	memset(this->mpVertexArray, 0, sizeof(ActionVertex)*this->mcMaxEvents);

	this->mpVertexPtrArray = new ActionPtr*[this->mcMaxEvents];

	// Create buffer for the vertecies
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
	// Copies vertex data from CPU over to the GPU
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
	// Copies camera data from CPU over to the GPU
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	this->pDirect3D->GetContext()->Map(
		this->mpGSBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);
	CameraData cameraData;
	XMVECTOR cameraPos = pCamera->GetPosition();
	DirectX::XMStoreFloat3(&cameraData.position, cameraPos);
	cameraData.data = (UINT)this->pMovableCamera->GetMovement();

	memcpy(
		mappedResource.pData,
		&cameraData,
		sizeof(CameraData));

	this->pDirect3D->GetContext()->Unmap(this->mpGSBuffer, 0);

	return true;
}
