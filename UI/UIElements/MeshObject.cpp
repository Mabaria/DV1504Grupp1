#include "MeshObject.h"

MeshObject::MeshObject(std::string name,
	std::vector<std::vector<unsigned int>> indices, 
	std::vector<std::vector<Vertex>> vertices)
{
	this->mName		= name;
	this->mIndices	= indices;
	this->mVertices	= vertices;

	// Doesn't matter which one determines the 
	// size as they are (should be) equal.
	this->mNumberOfBuffers = (int)indices.size();

	this->mModelMatrix = XMMatrixIdentity();
	this->mpConstantBuffer	= nullptr;
	this->mpTextureView		= nullptr;

	this->mpEventBuffer = nullptr;
}

MeshObject::~MeshObject()
{
	for (int i = 0; i < this->mpIndexBuffers.size(); i++)
	{
		if (this->mpIndexBuffers[i])
		{
			this->mpIndexBuffers[i]->Release();
			this->mpIndexBuffers[i] = nullptr;
		}
		if (this->mpVertexBuffers[i])
		{
			this->mpVertexBuffers[i]->Release();
			this->mpVertexBuffers[i] = nullptr;
		}
	}
	if (this->mpConstantBuffer)
	{
		this->mpConstantBuffer->Release();
		this->mpConstantBuffer = nullptr;
	}
	if (this->mpTextureView)
	{
		this->mpTextureView->Release();
		this->mpTextureView = nullptr;
	}

	if (this->mpEventBuffer)
	{
		this->mpEventBuffer->Release();
		this->mpEventBuffer = nullptr;
	}
}

const void MeshObject::Translate(float x, float y, float z)
{
	this->mModelMatrix *= XMMatrixTranslation(x, y, z);
}

const void MeshObject::Rotate(float pitch, float yaw, float roll)
{
	this->mModelMatrix *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

const void MeshObject::Scale(float x, float y, float z)
{
	this->mModelMatrix *= XMMatrixScaling(x, y, z);
}

const std::vector<std::vector<unsigned int>> MeshObject::GetIndices() const
{
	return this->mIndices;
}

const std::vector<std::vector<Vertex>> MeshObject::GetVertices() const
{
	return this->mVertices;
}

ID3D11Buffer** MeshObject::pGetVertexBuffer(int index)
{
	return &this->mpVertexBuffers[index];
}

ID3D11Buffer** MeshObject::pGetIndexBuffer(int index)
{
	return &this->mpIndexBuffers[index];
}

const int MeshObject::GetNumberOfBuffers() const
{
	return this->mNumberOfBuffers;
}

const std::string MeshObject::GetName() const
{
	return this->mName;
}

const void MeshObject::AddVertexBuffer(ID3D11Buffer **vertexBuffer)
{
	this->mpVertexBuffers.push_back(*vertexBuffer);
}

const void MeshObject::AddIndexBuffer(ID3D11Buffer **indexBuffer)
{
	this->mpIndexBuffers.push_back(*indexBuffer);
}

ID3D11Buffer ** MeshObject::rGetConstantBuffer()
{
	return &this->mpConstantBuffer;
}

ID3D11ShaderResourceView ** MeshObject::rGetTextureView()
{
	return &this->mpTextureView;
}

const void MeshObject::SetConstantBuffer(ID3D11Buffer ** constantBuffer)
{
	this->mpConstantBuffer = *constantBuffer;
}

XMMATRIX* MeshObject::rGetModelMatrix()
{
	return &this->mModelMatrix;
}

ID3D11Buffer ** MeshObject::rGetEventBuffer()
{
	return &this->mpEventBuffer;
}

void MeshObject::SetEvent(const EventData & active_events, ID3D11DeviceContext* context)
{
	D3D11_MAPPED_SUBRESOURCE data_ptr{};
	context->Map(
		this->mpEventBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&data_ptr);

	memcpy(data_ptr.pData, &active_events, sizeof(EventData));
	context->Unmap(this->mpEventBuffer, 0);
}
