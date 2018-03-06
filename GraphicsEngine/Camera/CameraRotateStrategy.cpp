#include "CameraRotateStrategy.h"

CameraRotateStrategy::CameraRotateStrategy()
{
	XMStoreFloat3(&this->mDirection, { 0.0f, 0.0f, 0.0f });
	XMStoreFloat3(&this->mUp, { 0.0f, 0.0f, 0.0f });

	this->mMaxHeight = 0.8f;
	this->mMinHeight = 0.3f;

	this->mZoomIn = false;
}

CameraRotateStrategy::~CameraRotateStrategy()
{
}

bool CameraRotateStrategy::Initialize(Camera & rCamera)
{
	CameraMovementStrategy::Initialize(rCamera);

	XMVECTOR dir = { 0.0f, 0.0f, 0.0f };
	XMVECTOR up = { 0.0f, 0.0f, 0.0f };
	
	// Get camera values to handle the camera correctly
	dir =
		this->pCamera->GetPosition() - this->pCamera->GetLookVector();

	up = this->pCamera->GetUpVector();

	this->mDistance = XMVectorGetX(
		XMVector3Length(dir)
	);

	dir = XMVector3Normalize(dir);

	XMStoreFloat3(&this->mUp, up);
	XMStoreFloat3(&this->mDirection, dir);

	return true;
}

void CameraRotateStrategy::Zoom(int zoom)
{
	this->mAnimate = false;

	float d = this->mDistance;
	d -= zoom * this->mZoomSpeed;

	// Keep zoom inside desired area
	
	if (d < this->mMinDistance)
	{
		d = this->mMinDistance;
	}

	else if (d > this->mMaxDistance)
	{
		d = this->mMaxDistance;
	}

	this->mDistance = d;
	this->mUpdatePosition();
}

void CameraRotateStrategy::Move(Position move)
{
	XMVECTOR dir = XMLoadFloat3(&this->mDirection);
	XMVECTOR up = XMLoadFloat3(&this->mUp);

	this->mAnimate = false;

	XMVECTOR right = XMVector3Normalize(
		XMVector3Cross(
			up,
			dir
		)
	);

	// Avoid movement in Y axis (Simple fix)
	right = XMVector3Normalize(DirectX::XMVectorSetY(right, 0));


	up = XMVector3Normalize(
		DirectX::XMVector3Cross(
			dir,
			right
		)
	);

	// Avoid movement being to high up and low
	float y = XMVectorGetY(dir);
	
	if (y < this->mMinHeight && move.y < 0)
		move.y = 0;

	else if (y > this->mMaxHeight && move.y > 0)
		move.y = 0;


	// Move along the right and up vectors based on mouse movement
	XMVECTOR newPos = this->pCamera->GetPosition()
		+ (right		* (float)move.x * this->mMoveSpeed * this->mDistance)
		+ (up	* (float)move.y * this->mMoveSpeed * this->mDistance);

	// Get new direction based on a normalized vector 
	// from point (look at) to new position
	dir = XMVector3Normalize(
		newPos - this->pCamera->GetLookVector()
	);

	XMStoreFloat3(&this->mDirection, dir);
	XMStoreFloat3(&this->mUp, up);

	this->mUpdatePosition();

}

void CameraRotateStrategy::FocusRoom(Room *pRoom, bool animate)
{
	this->mAnimate = animate;

	XMVECTOR new_pos = XMLoadFloat3(&pRoom->GetRoomCenter());
	new_pos = XMVectorSetX(new_pos, XMVectorGetX(new_pos) - (pRoom->GetRoomSize().x / 4.0f));
	

	if (this->mAnimate)
	{
		XMStoreFloat3(&this->mNewPosition, new_pos); 
		this->mZoomIn = true;
	}
	else
	{
		this->mDistance = this->mMinDistance;
		this->pCamera->SetLookVector(pRoom->GetRoomCenter());
		this->mUpdatePosition();
	}
}

void CameraRotateStrategy::Reset(bool animate)
{
	// Get default values and animate to it

	if (animate)
	{
		this->mAnimate = true;
		this->mZoomIn = false;
		XMVECTOR new_pos = this->pCamera->GetDefaultValues().look;
		XMStoreFloat3(&this->mNewPosition, new_pos);
	}
	else
	{
		this->mAnimate = false;
		this->pCamera->Reset();
		this->HandleChangeInCamera();
	}
}

void CameraRotateStrategy::HandleChangeInCamera()
{
	XMVECTOR dir = XMLoadFloat3(&this->mDirection);

	// Update member variables if camera data has been changed

	dir =
		this->pCamera->GetPosition() - this->pCamera->GetLookVector();

	this->mDistance = XMVectorGetX(
		XMVector3Length(dir)
	);

	dir = XMVector3Normalize(dir);

	XMStoreFloat3(&this->mDirection, dir);
}

void CameraRotateStrategy::AnimateToNewPosition()
{
	// Get new vector look at position and move it closer
	XMVECTOR dir = XMLoadFloat3(&this->mNewPosition) - this->pCamera->GetLookVector();
	this->pCamera->SetLookVector((dir * 0.2f) + this->pCamera->GetLookVector());
	
	// Whether to zoom in or out
	if (this->mZoomIn)
		this->mDistance -= (this->mDistance - this->mMinDistance) * 0.2f;
	else
		this->mDistance -= (this->mDistance - this->mMaxDistance) * 0.2f;
	

	if (XMVectorGetX(XMVector3Length(dir)) <= 0.00f)
	{
		this->mDistance = this->mZoomIn ? this->mMinDistance : this->mMaxDistance;
		this->mAnimate = false;
	}

	this->mUpdatePosition();
}

void CameraRotateStrategy::mUpdatePosition()
{
	this->pCamera->SetCameraPosition(this->pCamera->GetLookVector() + (XMLoadFloat3(&this->mDirection) * this->mDistance));
}
