#include "CameraRotateStrategy.h"

CameraRotateStrategy::CameraRotateStrategy()
{
	this->mDirection	= { 0.0f, 0.0f, 0.0f };
	this->mUp			= { 0.0f, 0.0f, 0.0f };
	this->mLookAt		= { 0.0f, 0.0f, 0.0f };

	this->mMaxHeight = 0.8f;
	this->mMinHeight = 0.3f;
}

CameraRotateStrategy::~CameraRotateStrategy()
{
}

bool CameraRotateStrategy::Initialize(Camera & rCamera)
{
	CameraMovementStrategy::Initialize(rCamera);

	this->mDirection =
		this->pCamera->GetPosition() - this->pCamera->GetLookVector();

	this->mUp = this->pCamera->GetUpVector();
	this->mLookAt = this->pCamera->GetLookVector();

	this->mDistance = XMVectorGetX(
		XMVector3Length(this->mDirection)
	);

	this->mDirection = XMVector3Normalize(this->mDirection);

	return true;
}

bool CameraRotateStrategy::Zoom(int zoom)
{
	bool flag = true;

	float d = this->mDistance;
	d -= zoom * this->mZoomSpeed;

	if (d < this->mMinDistance)
	{
		d = this->mMinDistance;
		flag = false;
	}

	if (d > this->mMaxDistance)
	{
		d = this->mMaxDistance;
		flag = false;
	}

	XMVECTOR v = this->pCamera->GetLookVector();
	this->pCamera->SetCameraPosition(v + (this->mDirection * d));

	this->mDistance = d;

	return flag;
}

bool CameraRotateStrategy::Move(Position move)
{
	bool flag = true;

	XMVECTOR right = XMVector3Normalize(
		XMVector3Cross(
			this->mUp,
			this->mDirection
		)
	);

	right = XMVector3Normalize(DirectX::XMVectorSetY(right, 0));


	this->mUp = XMVector3Normalize(
		DirectX::XMVector3Cross(
			this->mDirection,
			right
		)
	);

	float y = XMVectorGetY(this->mDirection);
	
	if (y < this->mMinHeight && move.y < 0)
		move.y = 0;

	else if (y > this->mMaxHeight && move.y > 0)
		move.y = 0;


	XMVECTOR newPos = this->pCamera->GetPosition()
		+ (right		* move.x * this->mMoveSpeed * this->mDistance)
		+ (this->mUp	* move.y * this->mMoveSpeed * this->mDistance);

	this->mDirection = XMVector3Normalize(
		newPos - this->mLookAt
	);

	XMVECTOR v = this->pCamera->GetLookVector();
	this->pCamera->SetCameraPosition(v + (this->mDirection * this->mDistance));

	return flag;
}

void CameraRotateStrategy::HandleChangeInCamera()
{
	this->mDirection =
		this->pCamera->GetPosition() - this->pCamera->GetLookVector();

	this->mDistance = XMVectorGetX(
		XMVector3Length(this->mDirection)
	);

	this->mDirection = XMVector3Normalize(this->mDirection);
}
