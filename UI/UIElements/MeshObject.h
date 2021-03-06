#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../GraphicsEngine/Parsing/Mesh.h"
#include "../../GraphicsEngine/Parsing/MaterialHandler.h"

using namespace DirectX;

// Fire				= 1
// Dead person		= 2
// Water			= 3
// Contamination	= 4

struct EventData
{
	float slots[4];
};

struct HoverData
{
	int IsEffectActive;
	int IsSelected;
	int pad0;
	int pad1;
};

class MeshObject
{
public:
	MeshObject(
		std::string name,
		Mesh* mesh);
	MeshObject(
		const MeshObject &other);
	MeshObject(
		const std::string name,
		std::vector<std::vector<unsigned int>> indices,
		std::vector<std::vector<Vertex>> vertices
	);
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
	const int GetNumberOfIndexBuffers() const;
	const int GetNumberOfMaterialBuffers() const;
	const std::string GetName() const;

	// Buffer adders add the parameter buffer to the end of the 
	// vector of buffers.
	const void AddVertexBuffer(ID3D11Buffer **vertexBuffer);
	const void AddIndexBuffer(ID3D11Buffer **indexBuffer);
	const void AddMaterialBuffer(ID3D11Buffer **materialBuffer);

	// For passing the model matrix to the shader.
	ID3D11Buffer **rGetMatrixBuffer();
	// For passing the material struct to the shader
	ID3D11Buffer **rGetMaterialBuffer(int index);

	ID3D11ShaderResourceView **rGetTextureView();

	const void SetMatrixBuffer(ID3D11Buffer **matrixBuffer);

	XMMATRIX *rGetModelMatrix();

	ID3D11Buffer **rGetEventBuffer(const unsigned int index);
	void SetEvent(const EventData& active_events,
		ID3D11DeviceContext* context,
		unsigned int index);

	ID3D11Buffer **rGetHoverBuffer(const unsigned int index);
	void SetHover(const bool hover,
		ID3D11DeviceContext* context,
		unsigned int index);

	void SetSelected(const bool selected,
		ID3D11DeviceContext *pContext,
		unsigned int index);

	MaterialHandler* pGetMaterialHandler();
	int GetMaterialIndexForIndexBuffer(unsigned int indexBufferIndex) const;

	void SetPixelShaderID(int id);
	int GetPixelShaderID();

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
	std::vector<ID3D11Buffer*> mpMaterialBuffers;
	ID3D11Buffer *mpMatrixBuffer;

	ID3D11ShaderResourceView *mpTextureView;

	int mNumberOfIndexBuffers;
	int mNumberOfMaterialBuffers;

	int mPixelShaderID;		// Pixel Shader to use

	XMMATRIX mModelMatrix;

	ID3D11Buffer *mpHoverBuffers[20];
	ID3D11Buffer* mpEventBuffers[20];
	bool mIsHovered[20];
	bool mIsSelected[20];
};