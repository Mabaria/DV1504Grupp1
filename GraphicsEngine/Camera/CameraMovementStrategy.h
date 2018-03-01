#pragma once

#include "Camera.h"
#include "../../IO/Mouse.h"
#include "../../Logic/Boat/Room.h"

using namespace DirectX;

class CameraMovementStrategy
{
public:
	CameraMovementStrategy();
	~CameraMovementStrategy();

	virtual bool Initialize(Camera& rCamera);
	virtual void Zoom(int zoom)							= 0;
	virtual void Move(Position move)					= 0;
	virtual void FocusRoom(Room *pRoom, bool animate)	= 0;
	virtual void Reset()								= 0;
	virtual void HandleChangeInCamera()					= 0;
	virtual void AnimateToNewPosition()					= 0;

	bool Animate();

private:
	
protected:
	bool mAnimate;
	XMFLOAT3 mNewPosition;

	float mMoveSpeed;
	float mZoomSpeed;

	float mDistance; // How far away the camera is from object
	float mMaxDistance;
	float mMinDistance;

	Camera* pCamera;
};
