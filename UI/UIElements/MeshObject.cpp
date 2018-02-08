#include "MeshObject.h"

MeshObject::MeshObject(std::string name,
	Mesh* mesh) : mMaterialHandler{ mesh->GetScenePointer() }
{
	this->mMesh = mesh;
	this->mName		= name;
	this->mIndices	= mesh->GetIndexVectors();
	this->mVertices	= mesh->GetVertexVectors();

	// Doesn't matter which one determines the 
	// size as they are (should be) equal.
	this->mNumberOfIndexBuffers = (int)this->mIndices.size();
	this->mNumberOfMaterialBuffers = this->mMaterialHandler.GetNumberOfMaterials();

	this->mModelMatrix = XMMatrixIdentity();
	this->mpMatrixBuffer	= nullptr;
	this->mpTextureView		= nullptr;
}

MeshObject::MeshObject(const MeshObject & other) 
	: mMaterialHandler{ other.mMesh->GetScenePointer() }
{
	this->mMesh = other.mMesh;
	this->mName = other.mName;
	this->mIndices = other.mIndices;
	this->mVertices = other.mVertices;

	this->mNumberOfIndexBuffers = other.mNumberOfIndexBuffers;
	this->mNumberOfMaterialBuffers = other.mNumberOfMaterialBuffers;

	this->mModelMatrix = other.mModelMatrix;
	this->mpMatrixBuffer = other.mpMatrixBuffer;
	this->mpMaterialBuffers = other.mpMaterialBuffers;
	this->mpTextureView = other.mpTextureView;
}

MeshObject::~MeshObject()
{
	for (unsigned int i = 0; i < this->mpIndexBuffers.size(); i++)
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
	for (unsigned int i = 0; i < this->mpMaterialBuffers.size(); i++)
	{
		if (this->mpMaterialBuffers[i])
		{
			this->mpMaterialBuffers[i]->Release();
			this->mpMaterialBuffers[i] = nullptr;

		}
	}
	if (this->mpMatrixBuffer)
	{
		this->mpMatrixBuffer->Release();
		this->mpMatrixBuffer = nullptr;
	}
	if (this->mpTextureView)
	{
		this->mpTextureView->Release();
		this->mpTextureView = nullptr;
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

const int MeshObject::GetNumberOfIndexBuffers() const
{
	return this->mNumberOfIndexBuffers;
}

const int MeshObject::GetNumberOfMaterialBuffers() const
{
	return this->mNumberOfMaterialBuffers;
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

const void MeshObject::AddMaterialBuffer(ID3D11Buffer ** materialBuffer)
{
	this->mpMaterialBuffers.push_back(*materialBuffer);
}

ID3D11Buffer ** MeshObject::rGetMatrixBuffer()
{
	return &this->mpMatrixBuffer;
}

ID3D11Buffer ** MeshObject::rGetMaterialBuffer(int index)
{
	return &this->mpMaterialBuffers[index];
}

ID3D11ShaderResourceView ** MeshObject::rGetTextureView()
{
	return &this->mpTextureView;
}

const void MeshObject::SetMatrixBuffer(ID3D11Buffer ** matrixBuffer)
{
	this->mpMatrixBuffer = *matrixBuffer;
}

XMMATRIX* MeshObject::rGetModelMatrix()
{
	return &this->mModelMatrix;
}

MaterialHandler * MeshObject::pGetMaterialHandler()
{
	return &this->mMaterialHandler;
}

int MeshObject::GetMaterialIndexForIndexBuffer(unsigned int indexBufferIndex) const
{
	return this->mMesh->GetSubmeshMaterialIndex(indexBufferIndex);
}
