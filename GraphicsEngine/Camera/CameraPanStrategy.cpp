#include "CameraPanStrategy.h"


#include <iostream>

CameraPanStrategy::CameraPanStrategy()
{
	this->mMinX = 
		this->mMinZ = 
		this->mMaxX = 
		this->mMaxZ = 0.0f;
}

CameraPanStrategy::~CameraPanStrategy()
{
}

bool CameraPanStrategy::Initialize(Camera & rCamera)
{
	CameraMovementStrategy::Initialize(rCamera);

	// Keep start position to limit moves outside of desired area
	this->mMinX = XMVectorGetX(this->pCamera->GetPosition()) - 2.0f;
	this->mMinZ = XMVectorGetZ(this->pCamera->GetPosition()) - 2.0f;

	this->mMaxX = XMVectorGetX(this->pCamera->GetPosition()) + 2.0f;
	this->mMaxZ = XMVectorGetZ(this->pCamera->GetPosition()) + 2.0f;

	this->mMoveSpeed = 0.0001f;

	return false;
}

void CameraPanStrategy::Zoom(int zoom)
{
	this->mAnimate = false;

	XMVECTOR pos = this->pCamera->GetPosition();
	this->mDistance = XMVectorGetY(pos);
	this->mDistance -= zoom * this->mZoomSpeed;

	// Keep zoom inside desired limits
	if (this->mDistance  < this->mMinDistance)
	{
		this->mDistance = this->mMinDistance;
	}
	else if (this->mDistance  > this->mMaxDistance)
	{
		this->mDistance = this->mMaxDistance;
	}

	// Set new zoom
	pos = XMVectorSetY(pos, this->mDistance);
	this->pCamera->SetCameraPosition(pos);
}

void CameraPanStrategy::Move(Position move)
{
	this->mAnimate = false;

	// Make the move
	this->pCamera->MoveCamera(
		-move.x * this->mDistance,
		0.0f,
		move.y * this->mDistance,
		-this->mMoveSpeed
	);

	XMVECTOR pos = this->pCamera->GetPosition();

	/* 
	
	Keep movement inside of the desired area if a move was
	made outside of the area

	*/
	float z = XMVectorGetZ(pos);
	float x = XMVectorGetX(pos);

	if (z < this->mMinZ)
	{
		pos = XMVectorSetZ(pos, this->mMinZ);
	}
	else if (z > this->mMaxZ)
	{
		pos = XMVectorSetZ(pos, this->mMaxZ);
	}

	if (x < this->mMinX)
	{
		pos = XMVectorSetX(pos, this->mMinX);
	}
	else if (x > this->mMaxX)
	{
		pos = XMVectorSetX(pos, this->mMaxX);
	}

	this->pCamera->SetCameraPosition(pos);
}

void CameraPanStrategy::FocusRoom(Room *pRoom, bool animate)
{
	this->mAnimate = animate;

	// Get position for the room picked
	XMVECTOR new_pos = XMLoadFloat3(&pRoom->GetRoomCenter());
	this->mDistance = this->mMinDistance;
	new_pos = XMVectorSetY(new_pos, this->mDistance);

	if (this->mAnimate)
	{
		XMStoreFloat3(&this->mNewPosition, new_pos);
	}
	else
	{
		this->pCamera->SetCameraPosition(new_pos);
	}
}

void CameraPanStrategy::Reset()
{
	// Get position for the reseted camera position
	this->HandleChangeInCamera();
	this->mAnimate = false;
}

void CameraPanStrategy::HandleChangeInCamera()
{
	// If a change has been made to the camera, then update the member variables

	XMVECTOR pos = this->pCamera->GetPosition();
	this->mDistance = XMVectorGetY(pos);
}

void CameraPanStrategy::AnimateToNewPosition()
{
	// Get direction and move to the position

	XMVECTOR dir = XMLoadFloat3(&this->mNewPosition) - this->pCamera->GetPosition();
	this->pCamera->MoveCamera(dir, this->mMoveSpeed * 1500.0f);
	this->mDistance = XMVectorGetY(this->pCamera->GetPosition());

	if (XMVectorGetX(XMVector3Length(dir)) <= 0.01f)
		this->mAnimate = false;
}
