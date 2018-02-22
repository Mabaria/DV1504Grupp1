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
	for (int i = 0; i < 20; i++)
	{
		this->mpEventBuffers[i] = nullptr;
	}
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
	for (int i = 0; i < 20; i++)
	{
		this->mpEventBuffers[i] = other.mpEventBuffers[i];
	}
}

MeshObject::MeshObject(const std::string name,
	std::vector<std::vector<unsigned int>> indices,
	std::vector<std::vector<Vertex>> vertices) : mMaterialHandler{ nullptr }
{
	this->mName = name;
	this->mIndices = indices;
	this->mVertices = vertices;

	// Doesn't matter which one determines the 
	// size as they are (should be) equal.
	this->mNumberOfIndexBuffers = (int)indices.size();
	this->mNumberOfMaterialBuffers = 0;

	this->mModelMatrix = XMMatrixIdentity();
	this->mpMatrixBuffer = nullptr;
	this->mpTextureView = nullptr;

	for (int i = 0; i < 20; i++)
	{
		this->mpEventBuffers[i] = nullptr;
	}

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

	for (int i = 0; i < 20; i++)
	{
		if (this->mpEventBuffers[i] != nullptr)
		{
			this->mpEventBuffers[i]->Release();
			this->mpEventBuffers[i] = nullptr;
		}
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
	if (this->mMesh == nullptr)
	{
		this->mNumberOfMaterialBuffers++;
	}
}

ID3D11Buffer ** MeshObject::rGetMatrixBuffer()
{
	return &this->mpMatrixBuffer;
}

ID3D11Buffer ** MeshObject::rGetMaterialBuffer(int index)
{
	if (index < this->mNumberOfMaterialBuffers)
		return &this->mpMaterialBuffers[index];
	else
		return nullptr;
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

ID3D11Buffer ** MeshObject::rGetEventBuffer(const unsigned int index)
{
	return &this->mpEventBuffers[index];
}

void MeshObject::SetEvent(const EventData & active_events,
	ID3D11DeviceContext* context,
	unsigned int index)
{
	D3D11_MAPPED_SUBRESOURCE data_ptr{};
	context->Map(
		this->mpEventBuffers[index],
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&data_ptr);

	memcpy(data_ptr.pData, &active_events, sizeof(EventData));
	context->Unmap(this->mpEventBuffers[index], 0);
}

MaterialHandler * MeshObject::pGetMaterialHandler()
{
	return &this->mMaterialHandler;
}

int MeshObject::GetMaterialIndexForIndexBuffer(unsigned int indexBufferIndex) const
{
	if (this->mMesh)
	{
		return this->mMesh->GetSubmeshMaterialIndex(indexBufferIndex);
	}
	return 0;
}

void MeshObject::SetPixelShaderID(int id)
{
	this->mPixelShaderID = id;
}

int MeshObject::GetPixelShaderID()
{
	return this->mPixelShaderID;
}
