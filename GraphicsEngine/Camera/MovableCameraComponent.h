#pragma once

#include "../../IO/Mouse.h"
#include "CameraPanStrategy.h"
#include "CameraRotateStrategy.h"
#include "../../IO/ObserverPattern/Observer.h"

enum CAMERA_MOVEMENT
{
	CAMERA_MOVEMENT_PAN = 0,
	CAMERA_MOVEMENT_ROTATE = 1,
};
class MovableCameraComponent : public Observer<std::string>
{
public:
	MovableCameraComponent();
	~MovableCameraComponent();

	bool Initialize(Camera& rCamera);
	bool Update();
	void Update(std::string * msg);
	bool UpdateAnimation();
	void FocusCameraOnRoom(Room* pRoom, bool use_animation);
	CAMERA_MOVEMENT GetMovement();

private:

	bool mUpdateInput();

	CameraMovementStrategy *mpStrategy;
	Buttons mBtnToMove;

	Position mMouseOrigin;
	Position mMouseDiff;

	Camera *pCamera;

	CAMERA_MOVEMENT mMovement;
};

