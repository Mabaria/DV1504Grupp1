#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
using namespace DirectX;

struct Vertex // Temporary struct until parsing is done.
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;
};

class MeshObject
{
public:
	MeshObject(
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

	// Buffer setters set the last buffer in the vector to the buffer parameter.
	const void SetVertexBuffer(ID3D11Buffer **vertexBuffer);
	const void SetIndexBuffer(ID3D11Buffer **indexBuffer);

	// Buffer adders add an empty buffer pointer to the vector of buffers.
	const void AddIndexBuffer();
	const void AddVertexBuffer();

private:
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<std::vector<Vertex>> mVertices;

	std::vector<ID3D11Buffer*> mpIndexBuffers;
	std::vector<ID3D11Buffer*> mpVertexBuffers;

	int mNumberOfBuffers;

	XMMATRIX mWorld;
};