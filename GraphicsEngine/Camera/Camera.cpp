#include "Camera.h"

Camera::Camera(const DirectX::XMVECTOR & r_position,
	const DirectX::XMVECTOR & r_up_vector,
	const DirectX::XMVECTOR & r_look_at)
{
	this->mInit(r_position, r_up_vector, r_look_at);
}

Camera::Camera(const DirectX::XMFLOAT3 & r_position,
	const DirectX::XMFLOAT3 & r_up_vector,
	const DirectX::XMFLOAT3 & r_look_at)
{
	this->mInit(DirectX::XMLoadFloat3(&r_position),
		DirectX::XMLoadFloat3(&r_up_vector),
		DirectX::XMLoadFloat3(&r_look_at));
}

Camera::Camera(const float pos_x, const float pos_y, const float pos_z, const float up_x, const float up_y, const float up_z, const float look_x, const float look_y, const float look_z)
{
	this->mInit(DirectX::XMVectorSet(pos_x, pos_y, pos_z, 0.0f),
		DirectX::XMVectorSet(up_x, up_y, up_z, 0.0f),
		DirectX::XMVectorSet(look_x, look_y, look_z, 0.0f));
}

Camera::~Camera()
{

}

void Camera::mInit(const DirectX::XMVECTOR & r_position,
	const DirectX::XMVECTOR & r_up_vector,
	const DirectX::XMVECTOR & r_look_at)
{
	this->mCameraPosition = r_position;
	this->mUpVector = r_up_vector;
	this->mLookAt = r_look_at;
	this->mUpdateViewMatrix();
}

void Camera::mUpdateViewMatrix()
{
	this->mViewMatrix = DirectX::XMMatrixLookAtLH(this->mCameraPosition,
		this->mLookAt,
		this->mUpVector);
}

void Camera::mRotateViewMatrix(const DirectX::XMMATRIX & camRotationMatrix) 
{
	//! IF SOMETHING IS BROKEN WITH CAMERA ROTATION THIS IS PROBABLY THE ROOT CAUSE
	this->mLookAt = 
		DirectX::XMVector3TransformCoord(this->mLookAt, camRotationMatrix);
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

void Camera::SetCameraPosition(const float new_x, const float new_y, const float new_z)
{
	this->mCameraPosition = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
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

void Camera::SetUpVector(const float new_x, const float new_y, const float new_z)
{
	this->mUpVector = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
}

void Camera::SetLookAt(const DirectX::XMFLOAT3 & r_new_look_at)
{
	this->mLookAt = DirectX::XMLoadFloat3(&r_new_look_at);
	this->mUpdateViewMatrix();
}

void Camera::SetLookAt(const DirectX::XMVECTOR & r_new_look_at)
{
	this->mLookAt = r_new_look_at;
	this->mUpdateViewMatrix();
}

void Camera::SetLookAt(const float new_x, const float new_y, const float new_z)
{
	this->mLookAt = DirectX::XMVectorSet(new_x, new_y, new_z, 0.0f);
	this->mUpdateViewMatrix();
}

void Camera::RotatePitchYawRoll(const float pitch, const float yaw, const float roll)
{
	this->mRotateViewMatrix(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));

}

void Camera::RotatePitchYawRoll(DirectX::XMFLOAT3 & pitch_yaw_roll)
{
	this->mRotateViewMatrix(
		DirectX::XMMatrixRotationRollPitchYawFromVector(
			DirectX::XMLoadFloat3(&pitch_yaw_roll)));
}

void Camera::RotatePitchYawRoll(DirectX::XMVECTOR & pitch_yaw_roll)
{
	this->mRotateViewMatrix(DirectX::XMMatrixRotationRollPitchYawFromVector(pitch_yaw_roll));
}

DirectX::XMVECTOR Camera::GetPosition() const
{
	return this->mCameraPosition;
}

DirectX::XMVECTOR Camera::GetUpVector() const
{
	return this->mUpVector;
}

DirectX::XMVECTOR Camera::GetLookAt() const
{
	return this->mLookAt;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return this->mViewMatrix;
}

DirectX::XMMATRIX Camera::GetTransposedViewMatrix() const
{
	return XMMatrixTranspose(this->mViewMatrix);
}
