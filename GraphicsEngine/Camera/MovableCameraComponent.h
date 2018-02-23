#pragma once

#include "../../IO/Mouse.h"
#include "CameraPanStrategy.h"
#include "CameraRotateStrategy.h"

enum CAMERA_MOVEMENT
{
	CAMERA_MOVEMENT_PAN = 0,
	CAMERA_MOVEMENT_ROTATE = 1,
};

class MovableCameraComponent
{
public:
	MovableCameraComponent();
	~MovableCameraComponent();

	bool Initialize(Camera& rCamera);
	bool Update();
	CAMERA_MOVEMENT GetMovement();

private:

	bool mUpdateInput();

	CameraMovementStrategy *mpStrategy;
	Buttons mBtnToMove;

	Position mMouseOrigin;
	Position mMouseDiff;

	CAMERA_MOVEMENT mMovement;
};

