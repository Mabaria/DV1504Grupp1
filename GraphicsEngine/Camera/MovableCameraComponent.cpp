#include "MovableCameraComponent.h"

MovableCameraComponent::MovableCameraComponent()
{
	this->mpStrategy	= nullptr;
	this->mBtnToMove	= Buttons::ScrollPress;

	this->mMouseDiff.x	= 0;
	this->mMouseDiff.y	= 0;

	this->mMouseOrigin	= this->mMouseDiff;
}

MovableCameraComponent::~MovableCameraComponent()
{
	if (this->mpStrategy)
	{
		delete this->mpStrategy;
		this->mpStrategy = nullptr;
	}
}

bool MovableCameraComponent::Initialize(Camera& rCamera)
{
	if (rCamera.GetProjectionMode() == ORTHOGRAPHIC)
	{
		MessageBoxA(NULL, 
			"Class Error: #MOVABLE_CAMERA_COMPONENT : Camera is not perspective!", 
			NULL, NULL);
		return false;
	}

	switch (rCamera.GetLookMode())
	{
	case LOOK_AT:
		this->mpStrategy = new CameraRotateStrategy();
		break;

	case LOOK_TO:
		this->mpStrategy = new CameraPanStrategy();
		break;
	}

	if (this->mpStrategy)
		this->mpStrategy->Initialize(rCamera);

	return true;
}

bool MovableCameraComponent::Update()
{
	bool flag = false;

	if (!this->mpStrategy)	// Early exit if it doesn't exists
		return flag;

	this->mpStrategy->HandleChangeInCamera();

	flag = this->mUpdateMouse();

	float zoom = Mouse::GetScroll();
	if (zoom)
		this->mpStrategy->Zoom(zoom);

	if (
		(this->mMouseDiff.x != 0 || this->mMouseDiff.y != 0) 
		&& Mouse::IsButtonDown(this->mBtnToMove)
		)
	{
		this->mpStrategy->Move(this->mMouseDiff);
		flag = true;
	}
	

	return !flag;
}

bool MovableCameraComponent::mUpdateMouse()
{
	bool flag = false;

	if (Mouse::IsButtonPressed(this->mBtnToMove))
	{
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		this->mMouseOrigin.x = (float)mouse_pos.x;
		this->mMouseOrigin.y = (float)mouse_pos.y;
	}

	if (Mouse::IsButtonDown(this->mBtnToMove))
	{
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);

		this->mMouseDiff.x = mouse_pos.x - this->mMouseOrigin.x;
		this->mMouseDiff.y = mouse_pos.y - this->mMouseOrigin.y;

		SetCursorPos(
			(int)this->mMouseOrigin.x,
			(int)this->mMouseOrigin.y
		);

		float dead_zone = 0.01f;

		if (abs(this->mMouseDiff.x) < dead_zone)
			this->mMouseDiff.x = 0;
		if (abs(this->mMouseDiff.y) < dead_zone)
			this->mMouseDiff.y = 0;

		flag = true;
	}

	return flag;
}



