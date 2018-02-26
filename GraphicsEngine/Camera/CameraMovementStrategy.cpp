#include "CameraMovementStrategy.h"


CameraMovementStrategy::CameraMovementStrategy()
{
	this->mMoveSpeed	= 0.005f;
	this->mZoomSpeed	= 0.5f;

	this->mDistance		= 5.0f;
	this->mMaxDistance	= 10.0f;
	this->mMinDistance	= 2.0f;

	this->pCamera		= nullptr;

	this->mAnimate = false;
}

CameraMovementStrategy::~CameraMovementStrategy()
{
}

bool CameraMovementStrategy::Initialize(Camera & rCamera)
{
	this->pCamera = &rCamera;
	this->mMaxDistance = this->pCamera->GetFarZ() - 4.0f;

	return false;
}

bool CameraMovementStrategy::Animate()
{
	return this->mAnimate;
}
