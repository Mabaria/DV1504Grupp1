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
	MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<std::vector<Vertex>> vertices);
	~MeshObject();

	const void Translate(float x, float y, float z);
	const void Rotate(float pitch, float yaw, float roll);
	const void Scale(float x, float y, float z);
	
	const std::vector<std::vector<unsigned int>> GetIndices() const;
	const std::vector<std::vector<Vertex>> GetVertices() const;

	ID3D11Buffer **pGetVertexBuffer(int index);
	ID3D11Buffer **pGetIndexBuffer(int index);
	const int GetNumberOfBuffers() const;

	const void SetVertexBuffer(int index, ID3D11Buffer *vertexBuffer);
	const void SetIndexBuffer(int index, ID3D11Buffer *indexBuffer);

	const void AddIndexBuffer(ID3D11Buffer* indexBuffer);
	const void AddVertexBuffer(ID3D11Buffer** vertexBuffer);

private:
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<std::vector<Vertex>> mVertices;

	std::vector<ID3D11Buffer*> mpIndexBuffers;
	std::vector<ID3D11Buffer*> mpVertexBuffers;

	int mNumberOfBuffers;

	XMMATRIX mWorld;
};