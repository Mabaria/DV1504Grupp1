#pragma once
#include "DX/Direct3D.h"
#include "Camera/Camera.h"
#include "Camera/MovableCameraComponent.h"
class Actions {
public:
	Actions();
	~Actions();

	void Init(D3D11 *pDirect3D);
	void SetCamera(Camera *pCamera);
	void SetMoveableCamera(MovableCameraComponent *pMovableCamera);
	bool AddAction(float x, float y, uint32_t data);
	void Draw();
private:
	struct ActionVertex
	{
		float x, y, z;
		uint32_t data;
	};
	struct CameraData
	{
		XMFLOAT3 position;
		uint32_t data;
	};
	const int mcMaxEvents = 2000;

	D3D11 *pDirect3D;
	Camera *pCamera;
	MovableCameraComponent *pMovableCamera;

	ID3D11VertexShader *mpVertexShader;
	ID3D11GeometryShader *mpGeometryShader;
	ID3D11PixelShader *mpPixelShader;
	ID3D11InputLayout *mpInputLayout;
	ID3D11ShaderResourceView *mpTextureResource;
	ID3D11SamplerState *mpSamplerState;
	ID3D11Buffer *mpGSBuffer;

	int mVertexSize;
	ActionVertex *mpVertexArray;
	ID3D11Buffer *mpVertexBuffer;
	bool mVertexArrayUpdated;

	bool CompileShadersAndLayout();
	bool CreateResourceAndSampler();
	bool CreateVertexBuffer();
	bool UpdateVertexBuffer();
	bool UpdateGSBuffer();
};