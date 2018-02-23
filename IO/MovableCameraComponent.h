#pragma once

#include "../IO/Mouse.h"
#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

struct MOVABLE_CAMERA_COMPONENT_DESC
{
	Camera* pCamera = nullptr;
};

class MovableCameraComponent
{
public:
	MovableCameraComponent();
	~MovableCameraComponent();

	bool Initialize(const MOVABLE_CAMERA_COMPONENT_DESC& desc);
	bool Update(Position move, int zoom);

private:

	bool mHandleLookAt(Position move, int zoom);
	bool mHandleLookTo(Position move, int zoom);

	Camera *mpCamera;
	LOOK_MODE mLookMode;

	XMVECTOR mUp;
	XMVECTOR mDirection;

	float mMoveSpeed;
	float mScrollSpeed;
	float mRadius;

	float mMaxDistance;
	float mMinDistance;

	Buttons mBtnToMove;
};

