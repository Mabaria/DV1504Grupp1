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
	MeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<Vertex> vertices);
	~MeshObject();

	const void Translate(float x, float y, float z);
	const void Rotate(float pitch, float yaw, float roll);
	
	const std::vector<std::vector<unsigned int>> GetIndices() const;
	const std::vector<Vertex> GetVertices() const;

	ID3D11Buffer* pGetVertexBuffer();
	ID3D11Buffer* pGetIndexBuffer(int index);

private:
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<Vertex> mVertices; // Pointer to the vector of all vertices.
	ID3D11Buffer **mpIndexBuffers;
	ID3D11Buffer *mpVertexBuffer;

	int mNumberOfIndexBuffers;

	XMMATRIX mWorld;
};