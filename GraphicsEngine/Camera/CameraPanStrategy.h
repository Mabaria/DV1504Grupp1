#pragma once

#include "CameraMovementStrategy.h"

class CameraPanStrategy : public CameraMovementStrategy
{
public:
	CameraPanStrategy();
	~CameraPanStrategy();

	virtual bool Initialize(Camera& rCamera);
	virtual bool Zoom(int zoom);
	virtual bool Move(Position move);
	virtual void HandleChangeInCamera();

private:
	XMVECTOR mStartPosition;
	float mMinX, 
		mMinZ, 
		mMaxX, 
		mMaxZ;
};
