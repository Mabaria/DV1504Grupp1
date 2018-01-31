#include "MeshObject.h"

MeshObject::MeshObject(std::string name,
	std::vector<std::vector<unsigned int>> indices, 
	std::vector<std::vector<Vertex>> vertices)
{
	this->mName		= name;
	this->mIndices	= indices;
	this->mVertices	= vertices;

	// Doesn't matter which one determines the size as they are (should be) equal.
	this->mNumberOfBuffers = (int)indices.size();
	this->mWorld		   = XMMatrixIdentity();
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