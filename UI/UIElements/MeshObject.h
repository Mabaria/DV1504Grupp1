#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../GraphicsEngine/Parsing/MaterialHandler.h"

using namespace DirectX;

class MeshObject
{
public:
	MeshObject(
		std::string name,
		Mesh* mesh);
	MeshObject(
		const MeshObject &other);
	~MeshObject();

	const void Translate(float x, float y, float z);
	const void Rotate(float pitch, float yaw, float roll);
	const void Scale(float x, float y, float z);
	
	// Data getters retrieve the full vector of vectors of data.
	const std::vector<std::vector<unsigned int>>	GetIndices() const;
	const std::vector<std::vector<Vertex>>			GetVertices() const;

	// Buffer getters retrieve the buffer at the index specified.
	ID3D11Buffer **pGetVertexBuffer(int index);
	ID3D11Buffer **pGetIndexBuffer(int index);

	// What could it possibly do?
	const int GetNumberOfBuffers() const;
	const std::string GetName() const;

	// Buffer adders add the parameter buffer to the end of the 
	// vector of buffers.
	const void AddVertexBuffer(ID3D11Buffer **vertexBuffer);
	const void AddIndexBuffer(ID3D11Buffer **indexBuffer);

	// For passing the model matrix to the shader.
	ID3D11Buffer **rGetMatrixBuffer();
	// For passing the material struct to the shader
	ID3D11Buffer **rGetMaterialBuffer();

	ID3D11ShaderResourceView **rGetTextureView();

	const void SetMatrixBuffer(ID3D11Buffer **matrixBuffer);
	const void SetMaterialBuffer(ID3D11Buffer **materialBuffer);

	XMMATRIX *rGetModelMatrix();

	void * operator new(size_t i) // To make sure it is 16 bit aligned
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void *p)
	{
		_aligned_free(p);
	}

private:
	std::string mName;
	Mesh* mMesh;
	MaterialHandler mMaterialHandler;
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<std::vector<Vertex>> mVertices;

	std::vector<ID3D11Buffer*> mpIndexBuffers;
	std::vector<ID3D11Buffer*> mpVertexBuffers;
	ID3D11Buffer *mpMatrixBuffer;
	ID3D11Buffer *mpMaterialBuffer;

	ID3D11ShaderResourceView *mpTextureView;


	int mNumberOfBuffers;

	XMMATRIX mModelMatrix;
};