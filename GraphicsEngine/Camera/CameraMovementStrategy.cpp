#include "CameraMovementStrategy.h"


CameraMovementStrategy::CameraMovementStrategy()
{
	this->mMoveSpeed	= 0.001f;
	this->mZoomSpeed	= 0.5f;

	this->mDistance		= 5.0f;
	this->mMaxDistance	= 10.0f;
	this->mMinDistance	= 1.0f;

	this->pCamera		= nullptr;
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
