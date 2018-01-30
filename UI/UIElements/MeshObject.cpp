#include "MeshObject.h"

MeshObject::MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<std::vector<Vertex>> vertices)
{
	this->mIndices				= indices;
	this->mVertices				= vertices;

	// Doesn't matter which one determines the size as they are equal.
	this->mNumberOfBuffers		= (int)indices.size();
	this->mWorld				= XMMatrixIdentity();
}

MeshObject::~MeshObject()
{
	for (int i = 0; i < this->mpIndexBuffers.size(); i++)
	{
		if (this->mpIndexBuffers[i])
		{
			//this->mpIndexBuffers[i]->Release();
		}
		if (this->mpVertexBuffers[i])
		{
			//this->mpVertexBuffers[i]->Release();
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

ID3D11Buffer ** MeshObject::pGetVertexBuffer(int index)
{
	ID3D11Buffer *to_return = nullptr;
	// If statement to avoid OOB.
	if (index >= 0 && index < this->mVertices.size())
	{
		to_return = this->mpVertexBuffers[index];
	}
	else
	{
		this->mpVertexBuffers.push_back(to_return);
		to_return = this->mpVertexBuffers.back();
	}
	return &to_return;
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

const int MeshObject::GetNumberOfBuffers() const
{
	return this->mNumberOfBuffers;
}

const void MeshObject::SetVertexBuffer(int index, ID3D11Buffer *vertexBuffer)
{
	if (index < this->mNumberOfBuffers)
	{
		this->mpVertexBuffers[index] = vertexBuffer;
	}
	else if (index == (this->mNumberOfBuffers + 1))
	{
		this->mpVertexBuffers.push_back(vertexBuffer);
	}
	else
	{
		MessageBoxA(NULL, "Vertex buffer out of bounds!", NULL, MB_OK);
	}
}

const void MeshObject::SetIndexBuffer(int index, ID3D11Buffer * indexBuffer)
{
	if (index < this->mNumberOfBuffers)
	{
		this->mpIndexBuffers[index] = indexBuffer;
	}
	else if (index == (this->mNumberOfBuffers + 1))
	{
		this->mpIndexBuffers.push_back(indexBuffer);
	}
	else
	{
		MessageBoxA(NULL, "Index buffer out of bounds!", NULL, MB_OK);
	}
}

const void MeshObject::AddIndexBuffer(ID3D11Buffer * indexBuffer)
{
	this->mpIndexBuffers.push_back(indexBuffer);
}

const void MeshObject::AddVertexBuffer(ID3D11Buffer ** vertexBuffer)
{
	this->mpVertexBuffers.push_back(*vertexBuffer);
}


