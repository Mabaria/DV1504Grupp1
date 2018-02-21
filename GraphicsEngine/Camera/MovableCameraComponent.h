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

private:

	bool mUpdateMouse();

	CameraMovementStrategy *mpStrategy;
	Buttons mBtnToMove;

	Position mMouseOrigin;
	Position mMouseDiff;
};

