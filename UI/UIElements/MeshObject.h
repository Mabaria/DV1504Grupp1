#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../GraphicsEngine/Parsing/Mesh.h"

using namespace DirectX;

class MeshObject
{
public:
	MeshObject(
		std::string name,
		std::vector<std::vector<unsigned int>> indices, 
		std::vector<std::vector<Vertex>> vertices);
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

private:
	std::string mName;
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<std::vector<Vertex>> mVertices;

	std::vector<ID3D11Buffer*> mpIndexBuffers;
	std::vector<ID3D11Buffer*> mpVertexBuffers;

	int mNumberOfBuffers;

	XMMATRIX mWorld;
};