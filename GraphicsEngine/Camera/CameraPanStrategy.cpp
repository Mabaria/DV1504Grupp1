#include "CameraPanStrategy.h"


#include <iostream>

CameraPanStrategy::CameraPanStrategy()
{
	this->mStartPosition = { 0.0f, 0.0f, 0.0f };

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
	this->mStartPosition = this->pCamera->GetPosition();

	this->mMinX = XMVectorGetX(this->mStartPosition) - 2.0f;
	this->mMinZ = XMVectorGetZ(this->mStartPosition) - 2.0f;

	this->mMaxX = XMVectorGetX(this->mStartPosition) + 2.0f;
	this->mMaxZ = XMVectorGetZ(this->mStartPosition) + 2.0f;

	this->mMoveSpeed = 0.0001f;

	return false;
}

bool CameraPanStrategy::Zoom(int zoom)
{
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

	return true;
}

bool CameraPanStrategy::Move(Position move)
{
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

	return true;
}

void CameraPanStrategy::HandleChangeInCamera()
{
	XMVECTOR pos = this->pCamera->GetPosition();
	this->mDistance = XMVectorGetY(pos);
}
