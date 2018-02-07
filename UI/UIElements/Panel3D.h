#pragma once

#include <Windows.h>
#include <vector>
#include <DDSTextureLoader.h>
#include "Panel.h"
#include "../../GraphicsEngine/DX/Direct3D.h"
#include "MeshObject.h"
#include "../../GraphicsEngine/Camera/Camera.h"

class Panel3D : public Panel
{
public:
	Panel3D(
		int width, 
		int height, 
		int top, 
		int left, 
		HWND handle, 
		LPCTSTR title);
	~Panel3D();
	
	D3D11& rGetDirect3D();

	// Creates buffers for and adds mesh 
	// objects into the vector of mesh objects.
	/*const void AddMeshObject(
		std::string name,
		std::vector<std::vector<unsigned int>> indices, 
		std::vector<std::vector<Vertex>> vertices,
		std::wstring texturePath);*/
	const void AddMeshObject(MeshObject *meshObject);

	// Pass through for the create shader function in D3D11,
	// setting shaders and setting up the input assembler.
	bool CreateShadersAndSetup(
		LPCWSTR vertexShaderPath,
		LPCWSTR geometryShaderPath,
		LPCWSTR pixelShaderPath);

	const void Update();

	// Handles the drawing of the 
	// entire panel in one call.
	const void Draw();

	// Returns nullptr if there is no mesh object
	// of the parameter name.
	MeshObject* rGetMeshObject(std::string name);

	// Updates constant buffers with matrices.
	const void UpdateMatrixBuffer(std::string name);
	const void UpdateMatrixBuffer(XMMATRIX *matrix, ID3D11Buffer **buffer);
	const void UpdateMatrixBuffer(int index);

	const void SetCamera(Camera *camera);

private:
	D3D11 mDirect3D;
	std::vector<MeshObject*> mpMeshObjects;
	HWND mPanelWindow;

	ID3D11VertexShader *mpVertexShader;
	ID3D11GeometryShader *mpGeometryShader;
	ID3D11PixelShader *mpPixelShader;
	ID3D11InputLayout *mpInputLayout;
	
	D3D11_INPUT_ELEMENT_DESC mInputDesc[3];

	Camera *mpCamera;
	ID3D11Buffer *mpViewBuffer;
	ID3D11Buffer *mpProjBuffer;

	// Buffer creators make a buffer to set to the last slot of the vector
	// of buffers in the last mesh object of the vector of mesh objects.
	const void CreateVertexBuffer(std::vector<Vertex> vertices);
	const void CreateIndexBuffer(std::vector<unsigned int> indices);

	// Creates the constant buffer for the last added mesh object.
	const void CreateMatrixBuffer(
		XMMATRIX *matrix, 
		ID3D11Buffer **constantBuffer);

	const void CreateTexture(std::wstring texturePath);

};