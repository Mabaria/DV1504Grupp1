#include "MeshObject.h"

MeshObject::MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<Vertex> vertices)
{
	this->mIndices				= indices;
	this->mVertices				= vertices;
	this->mNumberOfIndexBuffers = indices.size();

	this->mWorld				= XMMatrixIdentity();
	this->mView					= XMMatrixIdentity();
	this->mProjection			= XMMatrixIdentity();

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

const void MeshObject::SetView(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ)
{
	XMVECTOR eye	= XMVectorSet(eyeX, eyeY, eyeZ, 0.0f);
	XMVECTOR lookAt = XMVectorSet(lookX, lookY, lookZ, 0.0f);
	XMVECTOR up		= XMVectorSet(upX, upY, upZ, 0.0f);

	this->mView = XMMatrixLookAtLH(eye, lookAt, up);
}

const void MeshObject::SetOrthographicProjection(float width, float height, float nearZ, float farZ)
{
	this->mProjection = XMMatrixOrthographicLH(width, height, nearZ, farZ);
}

const std::vector<std::vector<unsigned int>> MeshObject::GetIndices() const
{
	return this->mIndices;
}

const std::vector<Vertex> MeshObject::GetVertices() const
{
	return this->mVertices;
}

ID3D11Buffer * MeshObject::rGetVertexBuffer()
{
	return this->mpVertexBuffer;
}

ID3D11Buffer * MeshObject::rGetIndexBuffer(int index)
{
	ID3D11Buffer *to_return = nullptr;
	// If statement to avoid OOB.
	if (index > 0 && index < this->mIndices.size())
	{
		to_return = this->mpIndexBuffers[index];
	}
	return to_return;
}


