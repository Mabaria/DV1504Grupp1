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

	const bool AddMeshObject(std::vector<std::vector<unsigned int>> indices, std::vector<Vertex> vertices);

	const void Update();
	const void Draw();

private:
	D3D11 mDirect3D;
	std::vector<MeshObject> mMeshObjects;
	HWND mPanelWindow;

	const bool CreateVertexBuffer(std::vector<Vertex> vertices, ID3D11Buffer *vertexBuffer);
	const bool CreateIndexBuffer(std::vector<unsigned int> indices, ID3D11Buffer *indexBuffer);
};