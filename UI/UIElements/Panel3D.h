#pragma once

#include <Windows.h>
#include "Panel.h"
#include "../../BIS/BIS/Direct3D.h"
#include "MeshObject.h"
#include <vector>

class Panel3D : public Panel
{
public:
	Panel3D(int width, int height, int top, int left, HWND handle);
	~Panel3D();
	
	D3D11& rGetDirect3D();

	// Creates buffers for and adds mesh objects into the vector of mesh objects.
	const bool AddMeshObject
	(
		std::vector<std::vector<unsigned int>> indices, 
		std::vector<Vertex> vertices
	);

	// Pass through for the create shader function in D3D11,
	// setting shaders and setting up the input assembler.
	bool CreateShadersAndSetup(
		LPCWSTR vertexShaderPath,
		LPCWSTR geometryShaderPath,
		LPCWSTR pixelShaderPath,
		ID3D11VertexShader **pVertexshader,
		ID3D11GeometryShader **pGeometryShader,
		ID3D11PixelShader **pPixelShader,
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[],
		UINT nrOfElements,
		ID3D11InputLayout **pInputLayout);

	const void Update();

	// Handles the drawing of the 
	// entire panel in one call.
	const void Draw();
	

private:
	D3D11 mDirect3D;
	std::vector<MeshObject> mMeshObjects;
	HWND mPanelWindow;

	// Private functions because they should 
	// only be used within the class.
	const bool CreateVertexBuffer
	(
		std::vector<Vertex> vertices, 
		ID3D11Buffer *vertexBuffer
	);
	const bool CreateIndexBuffer
	(
		std::vector<unsigned int> indices, 
		ID3D11Buffer *indexBuffer
	);
};