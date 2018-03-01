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
	virtual void FocusRoom(Room *pRoom, bool animate);
	virtual void Reset();
	virtual void HandleChangeInCamera();
	virtual void AnimateToNewPosition();

private:
	float mMinX, 
		mMinZ, 
		mMaxX, 
		mMaxZ;
};
