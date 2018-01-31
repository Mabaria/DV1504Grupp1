#include "MeshObject.h"

MeshObject::MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<std::vector<Vertex>> vertices)
{
	this->mIndices				= indices;
	this->mVertices				= vertices;

	// Doesn't matter which one determines the size as they are (should be) equal.
	this->mNumberOfBuffers		= (int)indices.size();
	this->mWorld				= XMMatrixIdentity();
}

MeshObject::~MeshObject()
{
	for (int i = 0; i < this->mpIndexBuffers.size(); i++)
	{
		if (this->mpIndexBuffers[i])
		{
			this->mpIndexBuffers[i]->Release();
		}
		if (this->mpVertexBuffers[i])
		{
			this->mpVertexBuffers[i]->Release();
		}
	}
}

const void MeshObject::Translate(float x, float y, float z)
{
	this->mWorld *= XMMatrixTranslation(x, y, z);
}

const void MeshObject::Rotate(float pitch, float yaw, float roll)
{
	this->mWorld *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

const void MeshObject::Scale(float x, float y, float z)
{
	this->mWorld *= XMMatrixScaling(x, y, z);
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

const void MeshObject::SetVertexBuffer(ID3D11Buffer **vertexBuffer)
{
	this->mpVertexBuffers.back() = *vertexBuffer;
}

const void MeshObject::SetIndexBuffer(ID3D11Buffer **indexBuffer)
{
	this->mpIndexBuffers.back() = *indexBuffer;
}

const void MeshObject::AddIndexBuffer()
{
	this->mpIndexBuffers.push_back(nullptr);
}

const void MeshObject::AddVertexBuffer()
{
	this->mpVertexBuffers.push_back(nullptr);
}