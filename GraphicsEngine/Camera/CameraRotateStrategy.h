#pragma once

#include "CameraMovementStrategy.h"

class CameraRotateStrategy : public CameraMovementStrategy
{
public:
	CameraRotateStrategy();
	~CameraRotateStrategy();

	virtual bool Initialize(Camera& rCamera);
	virtual void Zoom(int zoom);
	virtual void Move(Position move);
	virtual void HandleChangeInCamera();

private:
	DirectX::XMVECTOR mUp;
	DirectX::XMVECTOR mDirection;	// The direction camera is displaced
	DirectX::XMVECTOR mLookAt;

	float mMaxHeight;
	float mMinHeight;

};
