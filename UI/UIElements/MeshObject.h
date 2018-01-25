#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
using namespace DirectX;

struct Vertex
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
	const void SetView(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);
	const void SetOrthographicProjection(float width, float height, float nearZ, float farZ);

	const std::vector<std::vector<unsigned int>> GetIndices() const;
	const std::vector<Vertex> GetVertices() const;

private:
	std::vector<std::vector<unsigned int>> mIndices;
	std::vector<Vertex> mVertices; // Pointer to the vector of all vertices.
	ID3D11Buffer **mpIndexBuffers;
	ID3D11Buffer *mpVertexBuffer;

	int mNumberOfIndexBuffers;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};