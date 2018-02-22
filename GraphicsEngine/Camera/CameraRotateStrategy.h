#pragma once

#include "CameraMovementStrategy.h"

class CameraRotateStrategy : public CameraMovementStrategy
{
public:
	CameraRotateStrategy();
	~CameraRotateStrategy();

	virtual bool Initialize(Camera& rCamera);
	virtual bool Zoom(int zoom);
	virtual bool Move(Position move);
	virtual void HandleChangeInCamera();

private:
	DirectX::XMVECTOR mUp;
	DirectX::XMVECTOR mDirection;	// The direction camera is displaced
	DirectX::XMVECTOR mLookAt;

	float mMaxHeight;
	float mMinHeight;

};
