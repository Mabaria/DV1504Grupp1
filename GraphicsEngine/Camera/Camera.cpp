#include "Camera.h"

Camera::Camera(const DirectX::XMVECTOR & r_position,
	const DirectX::XMVECTOR & r_up_vector,
	const DirectX::XMVECTOR & r_look_vector,
	const CAMERAMODE camera_mode)
{
	this->mInit(r_position, r_up_vector, r_look_vector, camera_mode);
}

Camera::Camera(const DirectX::XMFLOAT3 & r_position,
	const DirectX::XMFLOAT3 & r_up_vector,
	const DirectX::XMFLOAT3 & r_look_vector,
	const CAMERAMODE camera_mode)
{
	this->mInit(DirectX::XMLoadFloat3(&r_position),
		DirectX::XMLoadFloat3(&r_up_vector),
		DirectX::XMLoadFloat3(&r_look_vector),
		camera_mode);
}

Camera::Camera(const float pos_x,
	const float pos_y,
	const float pos_z,
	const float up_x,
	const float up_y,
	const float up_z,
	const float look_x,
	const float look_y,
	const float look_z,
	const CAMERAMODE camera_mode)
{
	this->mInit(DirectX::XMVectorSet(pos_x, pos_y, pos_z, 0.0f),
		DirectX::XMVectorSet(up_x, up_y, up_z, 0.0f),
		DirectX::XMVectorSet(look_x, look_y, look_z, 0.0f),
		camera_mode);
}

Camera::~Camera()
{

}

void Camera::mInit(const DirectX::XMVECTOR & r_position,
	const DirectX::XMVECTOR & r_up_vector,
	const DirectX::XMVECTOR & r_look_vector,
	const CAMERAMODE camera_mode)
{
	this->mCameraPosition = r_position;
	this->mUpVector = r_up_vector;
	this->mLookVector = r_look_vector;
	this->mCameraMode = camera_mode;
	this->mUpdateViewMatrix();
}

void Camera::mUpdateViewMatrix()
{
	if (this->mCameraMode == LOOKAT)
	{
		this->mViewMatrix = DirectX::XMMatrixLookAtLH(this->mCameraPosition,
			this->mLookVector,
			this->mUpVector);
	}
	else
	{
		this->mViewMatrix = DirectX::XMMatrixLookToLH(this->mCameraPosition,
			this->mLookVector,
			this->mUpVector);
	}
}

void Camera::mRotateViewMatrix(const DirectX::XMMATRIX & camRotationMatrix) 
{
	//! IF SOMETHING IS BROKEN WITH CAMERA ROTATION THIS IS PROBABLY THE ROOT CAUSE
	this->mLookVector = 
		DirectX::XMVector3TransformCoord(this->mLookVector, camRotationMatrix);
	this->mUpVector =
		DirectX::XMVector3TransformCoord(this->mUpVector, camRotationMatrix);
	this->mUpdateViewMatrix();
}

void Camera::SetCameraPosition(const DirectX::XMFLOAT3 & r_new_position)
{
	this->mCameraPosition = DirectX::XMLoadFloat3(&r_new_position);
	this->mUpdateViewMatrix();
}

void Camera::SetCameraPosition(const DirectX::XMVECTOR & r_new_position)
{
	this->mCameraPosition = r_new_position;
	this->mUpdateViewMatrix();
}

void Camera::SetCameraPosition(const float new_x, const float new_y,
	const float new_z)
{
	this->mCameraPosition = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
}

void Camera::MoveCamera(const DirectX::XMFLOAT3 & r_direction,
	const float distance)
{
	DirectX::XMVECTOR direction_vector = DirectX::XMLoadFloat3(&r_direction);
	DirectX::XMVECTOR displacement = 
		DirectX::XMVectorScale(direction_vector, distance);

	this->SetCameraPosition(
		DirectX::XMVectorAdd(this->mCameraPosition, displacement));
	
}

void Camera::MoveCamera(const DirectX::XMVECTOR & r_direction,
	const float distance)
{
	DirectX::XMVECTOR displacement =
		DirectX::XMVectorScale(r_direction, distance);

	this->SetCameraPosition(
		DirectX::XMVectorAdd(this->mCameraPosition, displacement));
}

void Camera::MoveCamera(const float direction_x,
	const float direction_y,
	const float direction_z,
	const float distance)
{
	DirectX::XMVECTOR displacement =
		DirectX::XMVectorScale(
			DirectX::XMVectorSet(direction_x,
			direction_y,
			direction_z,
			0.0f),
			distance);
	this->SetCameraPosition(
		DirectX::XMVectorAdd(this->mCameraPosition, displacement));
}

void Camera::SetUpVector(const DirectX::XMFLOAT3 & r_new_up)
{
	this->mUpVector = DirectX::XMLoadFloat3(&r_new_up);
	this->mUpdateViewMatrix();
}

void Camera::SetUpVector(const DirectX::XMVECTOR & r_new_up)
{
	this->mUpVector = r_new_up;
	this->mUpdateViewMatrix();
}

void Camera::SetUpVector(const float new_x,
	const float new_y,
	const float new_z)
{
	this->mUpVector = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
}

void Camera::SetLookVector(const DirectX::XMFLOAT3 & r_new_look_vector)
{
	this->mLookVector = DirectX::XMLoadFloat3(&r_new_look_vector);
	this->mUpdateViewMatrix();
}

void Camera::SetLookVector(const DirectX::XMVECTOR & r_new_look_vector)
{
	this->mLookVector = r_new_look_vector;
	this->mUpdateViewMatrix();
}

void Camera::SetLookVector(const float new_x, const float new_y, const float new_z)
{
	this->mLookVector = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
}

void Camera::SetCameraMode(CAMERAMODE new_cameramode)
{
	this->mCameraMode = new_cameramode;
}

void Camera::RotateCameraPitchYawRoll(const float pitch,
	const float yaw,
	const float roll)
{
	this->mRotateViewMatrix(
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));

}

void Camera::RotateCameraPitchYawRoll(const DirectX::XMFLOAT3 & pitch_yaw_roll)
{
	this->mRotateViewMatrix(
		DirectX::XMMatrixRotationRollPitchYawFromVector(
			DirectX::XMLoadFloat3(&pitch_yaw_roll)));
}

void Camera::RotateCameraPitchYawRoll(const DirectX::XMVECTOR & pitch_yaw_roll)
{
	this->mRotateViewMatrix(
		DirectX::XMMatrixRotationRollPitchYawFromVector(pitch_yaw_roll));
}

DirectX::XMVECTOR Camera::GetPosition() const
{
	return this->mCameraPosition;
}

DirectX::XMVECTOR Camera::GetUpVector() const
{
	return this->mUpVector;
}

DirectX::XMVECTOR Camera::GetLookVector() const
{
	return this->mLookVector;
}

CAMERAMODE Camera::GetCameraMode() const
{
	return this->mCameraMode;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return this->mViewMatrix;
}

DirectX::XMMATRIX Camera::GetTransposedViewMatrix() const
{
	return XMMatrixTranspose(this->mViewMatrix);
}