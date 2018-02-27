#pragma once

#include "../../IO/Mouse.h"
#include "CameraPanStrategy.h"
#include "CameraRotateStrategy.h"

class MovableCameraComponent
{
public:
	MovableCameraComponent();
	~MovableCameraComponent();

	bool Initialize(Camera& rCamera);
	bool Update();
	bool UpdateAnimation();
	void FocusCameraOnRoom(Room* pRoom, bool use_animation);

private:

	bool mUpdateInput();

	CameraMovementStrategy *mpStrategy;
	Buttons mBtnToMove;

	Position mMouseOrigin;
	Position mMouseDiff;
};

