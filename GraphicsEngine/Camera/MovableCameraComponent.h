#pragma once

#include "../../IO/Mouse.h"
#include "CameraPanStrategy.h"
#include "CameraRotateStrategy.h"
#include "../../IO/ObserverPattern/Observer.h"

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

private:

	bool mUpdateInput();

	CameraMovementStrategy *mpStrategy;
	Buttons mBtnToMove;

	Position mMouseOrigin;
	Position mMouseDiff;

	Camera *pCamera;
};

