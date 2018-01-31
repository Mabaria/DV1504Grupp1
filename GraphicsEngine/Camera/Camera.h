#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

enum CAMERAMODE {
	LOOKAT = 0,
	LOOKTO = 1
};

class Camera {
public:
	Camera(const DirectX::XMVECTOR &r_position,
		const DirectX::XMVECTOR &r_up_vector,
		const DirectX::XMVECTOR &r_look,
		const CAMERAMODE camera_mode = LOOKAT);
	Camera(const DirectX::XMFLOAT3 &r_position,
		const DirectX::XMFLOAT3 &r_up_vector,
		const DirectX::XMFLOAT3 &r_look_vector,
		const CAMERAMODE camera_mode = LOOKAT);
	Camera(const float pos_x,
		const float pos_y,
		const float pos_z,
		const float up_x,
		const float up_y,
		const float up_z,
		const float look_x,
		const float look_y,
		const float look_z,
		const CAMERAMODE camera_mode = LOOKAT);

	~Camera();

	void SetCameraPosition(const DirectX::XMFLOAT3 &r_new_position);
	void SetCameraPosition(const DirectX::XMVECTOR &r_new_position);
	void SetCameraPosition(const float new_x,
		const float new_y,
		const float new_z);

	void MoveCamera(const DirectX::XMFLOAT3 &r_direction,
		const float distance);
	void MoveCamera(const DirectX::XMVECTOR &r_direction,
		const float distance);
	void MoveCamera(const float direction_x,
		const float direction_y,
		const float direction_z,
		const float distance);

	void SetUpVector(const DirectX::XMFLOAT3 &r_new_up);
	void SetUpVector(const DirectX::XMVECTOR &r_new_up);
	void SetUpVector(const float new_x, const float new_y, const float new_z);

	void SetLookVector(const DirectX::XMFLOAT3 &r_new_look_vector);
	void SetLookVector(const DirectX::XMVECTOR &r_new_look_vector);
	void SetLookVector(const float new_x, const float new_y, const float new_z);

	void SetCameraMode(CAMERAMODE new_cameramode);

	void RotateCameraPitchYawRoll(
		const float pitch,
		const float yaw,
		const float roll);
	void RotateCameraPitchYawRoll(const DirectX::XMFLOAT3 &pitch_yaw_roll);
	void RotateCameraPitchYawRoll(const DirectX::XMVECTOR &pitch_yaw_roll);

	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetUpVector() const;
	DirectX::XMVECTOR GetLookVector() const;
	CAMERAMODE GetCameraMode() const;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetTransposedViewMatrix() const;

private:
	DirectX::XMVECTOR mCameraPosition;
	DirectX::XMVECTOR mUpVector;
	DirectX::XMVECTOR mLookVector;
	CAMERAMODE mCameraMode;
	DirectX::XMMATRIX mViewMatrix;
	void mInit(const DirectX::XMVECTOR & r_position,
		const DirectX::XMVECTOR & r_up_vector,
		const DirectX::XMVECTOR & r_look_vector,
		const CAMERAMODE camera_mode);
	void mUpdateViewMatrix();
	void mRotateViewMatrix(const DirectX::XMMATRIX &camRotationMatrix); 
	 //! IF SOMETHING IS BROKEN WITH CAMERA ROTATION
	 //! mRotateViewMatrix IS PROBABLY THE ROOT CAUSE
	 
};