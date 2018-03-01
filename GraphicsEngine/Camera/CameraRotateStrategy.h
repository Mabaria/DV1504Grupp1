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
	virtual void FocusRoom(Room *pRoom, bool animate);
	virtual void Reset();
	virtual void HandleChangeInCamera();
	virtual void AnimateToNewPosition();

private:
	XMFLOAT3 mUp;
	XMFLOAT3 mDirection;

	float mMaxHeight;
	float mMinHeight;

	void mUpdatePosition();
};
