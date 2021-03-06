#pragma once
#include "DX/Direct3D.h"
#include "Camera/Camera.h"
#include "Camera/MovableCameraComponent.h"
#include "../IO/Picking.h"

enum ActionData : uint32_t
{
	Icon_Injured_Moved = 0,
	Icon_Injured_Treated = 1,
	Icon_Injured_Reported = 2,
	Icon_Hole_In_Bulk = 3,
	Icon_Ventilation_In = 4,
	Icon_Ventilation_Out = 5,
	Icon_Cooling_Wall = 6,
	Icon_Supporting_Wall = 7,
	Icon_Damaged_Bulk = 8,
	Icon_Draining = 9,
	Icon_Seal_Hole = 10,
	Rotation_0 = 0,
	Rotation_90 = 16,
	Rotation_180 = 32,
	Rotation_270 = 48,
	Rotation_Stationary = 64,
	Number_0 = 0,
	Number_1 = 512,
	Number_2 = 1024,
	Number_3 = 1536,
	Number_4 = 2048,
	Number_5 = 2560,
	Number_6 = 3072,
	Number_7 = 3584,
	Number_8 = 4096,
	Number_9 = 4608,
	No_Action = 9001
};


class Actions {
public:

	//struct ActionPtr
	//{
	//	int index;
	//	ActionPtr(int index) {
	//		this->index = index;
	//	}
	//};

	struct Info
	{
		float x;
		float z;
		uint32_t data;
	};

	Actions();
	~Actions();

	bool Init(D3D11 *pDirect3D);
	void SetCamera(Camera *pCamera);
	void SetMoveableCamera(MovableCameraComponent *pMovableCamera);
	int *AddAction(float x, float y, uint32_t data);
	void RemoveAction(int **pActionPtr);
	int *PickAction();
	void Draw();

	

	void InitFromFile(std::vector<Info> input, std::vector<int*> &output);

	ID3D11ShaderResourceView *GetIconResource();
	ID3D11ShaderResourceView *GetNumberResource();

private:
	struct ActionVertex
	{
		float x, y, z;
		uint32_t data;
	};
	struct CameraData
	{
		XMFLOAT3 position;
		float aspectAndData;
	};
	const int mcMaxEvents = 500;

	D3D11 *pDirect3D;
	Camera *pCamera;
	MovableCameraComponent *pMovableCamera;

	ID3D11VertexShader *mpVertexShader;
	ID3D11GeometryShader *mpGeometryShader;
	ID3D11PixelShader *mpPixelShader;
	ID3D11InputLayout *mpInputLayout;
	ID3D11ShaderResourceView *mpIconResource;
	ID3D11ShaderResourceView *mpNumberResource;
	ID3D11SamplerState *mpSamplerState;
	ID3D11Buffer *mpGSBuffer;

	int mVertexSize;
	ActionVertex *mpVertexArray;
	int **mpVertexPtrArray;
	ID3D11Buffer *mpVertexBuffer;
	bool mVertexArrayUpdated;

	bool CompileShadersAndLayout();
	bool CreateResources();
	bool CreateVertexBuffer();
	bool UpdateVertexBuffer();
	bool UpdateGSBuffer();
};