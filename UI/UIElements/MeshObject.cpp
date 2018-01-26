#include "MeshObject.h"

MeshObject::MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<Vertex> vertices)
{
	this->mIndices				= indices;
	this->mVertices				= vertices;
	this->mNumberOfIndexBuffers = (int)indices.size();

	this->mWorld				= XMMatrixIdentity();

	this->mpVertexBuffer		= nullptr;
	this->mpIndexBuffers		= new ID3D11Buffer*[this->mNumberOfIndexBuffers];
	
	for (int i = 0; i < this->mNumberOfIndexBuffers; i++)
	{
		this->mpIndexBuffers[i] = nullptr;
	}
}

MeshObject::~MeshObject()
{
}

const void MeshObject::Translate(float x, float y, float z)
{
	this->mWorld *= XMMatrixTranslation(x, y, z);
}

const void MeshObject::Rotate(float pitch, float yaw, float roll)
{
	this->mWorld *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

const std::vector<std::vector<unsigned int>> MeshObject::GetIndices() const
{
	return this->mIndices;
}

const std::vector<Vertex> MeshObject::GetVertices() const
{
	return this->mVertices;
}

ID3D11Buffer ** MeshObject::pGetVertexBuffer()
{
	return &this->mpVertexBuffer;
}

ID3D11Buffer ** MeshObject::pGetIndexBuffer(int index)
{
	ID3D11Buffer **to_return = nullptr;
	// If statement to avoid OOB.
	if (index > 0 && index < this->mIndices.size())
	{
		to_return = &this->mpIndexBuffers[index];
	}
	return to_return;
}

const int MeshObject::GetNumberOfIndexBuffers() const
{
	return this->mNumberOfIndexBuffers;
}


