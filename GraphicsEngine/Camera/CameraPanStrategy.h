#pragma once

#include "CameraMovementStrategy.h"

class CameraPanStrategy : public CameraMovementStrategy
{
public:
	CameraPanStrategy();
	~CameraPanStrategy();

	virtual bool Initialize(Camera& rCamera);
	virtual void Zoom(int zoom);
	virtual void Move(Position move);
	virtual void HandleChangeInCamera();

private:
	XMVECTOR mStartPosition;
	float mMinX, 
		mMinZ, 
		mMaxX, 
		mMaxZ;
};
