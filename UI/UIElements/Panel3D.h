#pragma once

#include <Windows.h>
#include <vector>
#include <DDSTextureLoader.h>
#include "Panel.h"
#include "../../GraphicsEngine/DX/Direct3D.h"
#include "MeshObject.h"
#include "../../GraphicsEngine/Camera/Camera.h"
#include "../../IO/ObserverPattern/Observer.h"
#include "../../GraphicsEngine/DX/Direct2D.h"
#include "../../GraphicsEngine/Actions.h"
#include "../../GraphicsEngine/Camera/MovableCameraComponent.h"

enum PANEL3D_SHADER_TYPE
{
	PANEL3D_SHADER_BOAT = 1,
	PANEL3D_SHADER_TEXT = 2,
	PANEL3D_SHADER_EVENT = 3
};

class Panel3D : public Panel, public Observer<Button>
	
{
public:
	Panel3D(
		int width,
		int height,
		int top,
		int left,
		HWND handle,
		LPCTSTR title,
		bool movableCamera = false);
	~Panel3D();

	D3D11& rGetDirect3D();

	// Creates buffers for and adds mesh 
	// objects into the vector of mesh objects.
	const void AddMeshObject(
		std::string name,
		std::vector<std::vector<unsigned int>> indices,
		std::vector<std::vector<Vertex>> vertices,
		int pixelShaderID = 0,
		std::wstring texturePath = L"",
		bool use_event = false);

	const void AddMeshObject(MeshObject *meshObject,
		int pixelShaderID = 0,
		std::wstring texturePath = L"",
		bool use_event = false);

	// Pass through for the create shader function in D3D11,
	// setting shaders and setting up the input assembler.
	bool CreateShadersAndSetup(
		LPCWSTR vertexShaderPath,
		LPCWSTR geometryShaderPath,
		LPCWSTR pixelShaderPath);

	bool AddPixelShader(LPCWSTR pixelShaderPath);

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
	void Update(Button* attribute);

	Camera* GetActiveCamera();

	MovableCameraComponent* GetMovableComponent();

	void * operator new(size_t i) // To make sure it is 16 bit aligned
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void *p)
	{
		_aligned_free(p);
	}

	void BindTextureToBitmap(ID3D11Texture2D *texture);
	void DrawBitmapToTexture(
		ID2D1Bitmap *bitmap,
		float startX,
		float startY,
		float widthOfTex,
		float heightOfTex);
	void DrawBitmapToTexture(
		ID2D1Bitmap *bitmap,
		D2D1_RECT_F destRect,
		const D2D1_RECT_F sourceRect,
		float opacity = 1.0f,
		const D2D1_MATRIX_4X4_F *transform = nullptr);
	//void SetCameraPosition()


	void AddAction(float x, float y, ActionData data);

	void InitActions();
	Actions *pGetActions();

	const void SetActionHover(bool state);
	const void SetIcon(uint32_t data);

	// Rotates the ghost icon 90 degrees clockwise.
	const void RotateIcon();

	const void ResetIcon();

private:
	D3D11 mDirect3D;
	std::vector<MeshObject*> mpMeshObjects;
	//HWND mPanelWindow;

	ID3D11VertexShader *mpVertexShader;
	ID3D11GeometryShader *mpGeometryShader;
	ID3D11InputLayout *mpInputLayout;

	ID3D11PixelShader *mpPixelShader;
	std::vector<ID3D11PixelShader*> mpPixelShaders;
	
	D3D11_INPUT_ELEMENT_DESC mInputDesc[3];

	Camera *mpCamera;
	ID3D11Buffer *mpViewBuffer;
	ID3D11Buffer *mpProjBuffer;

	// Buffer creators make a buffer to set to the last slot of the vector
	// of buffers in the last mesh object of the vector of mesh objects.
	const void CreateVertexBuffer(std::vector<Vertex> vertices);
	const void CreateIndexBuffer(std::vector<unsigned int> indices);

	// Creates the constant buffer for the last added mesh object.
	const void mCreateMatrixBuffer(
		XMMATRIX *matrix,
		ID3D11Buffer **constantBuffer);

	const void mCreateMaterialBuffer(
		MaterialStruct *matStruct);


	const void CreateTexture(std::wstring texturePath);


	MovableCameraComponent* mpMovableCameraComponent;
	bool mShowCursor;
	bool mMovableCamera;

	Actions *mpActions;

	//! GHOST ACTION STUFF

	bool mUsingGhosts;

	// Back buffer texture for hijacking purposes.
	ID3D11Texture2D *mpBackBuffer;

	// Bitmaps containing the icons and numbers.
	ID2D1Bitmap	*mpIconBitmap;
	ID2D1Bitmap	*mpNumberBitmap;

	// Rectangle to draw from.
	D2D1_RECT_F mGhostIconRect;
	D2D1_RECT_F mGhostNumberRect;

	// Center position of the ghost.
	D2D1_RECT_F mGhostPosition;

	// Rotation/scale for the ghost.
	D2D1_MATRIX_4X4_F mGhostTransform;
	int mGhostRotation;
	float mGhostScale;

	// Whether or not the ghost should be shown.
	bool mGhostActive;

	// Whether or not the ghost should rotate.
	bool mGhostStationary;

	const void mUpdateGhostTransform();
	const void mUpdateGhosts();
	const void mDrawGhosts();
};