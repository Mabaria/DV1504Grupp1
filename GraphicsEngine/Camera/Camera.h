#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Camera {
public:
	Camera(const DirectX::XMVECTOR &r_position,
		const DirectX::XMVECTOR &r_up_vector,
		const DirectX::XMVECTOR &r_look_at);
	Camera(const DirectX::XMFLOAT3 &r_position,
		const DirectX::XMFLOAT3 &r_up_vector,
		const DirectX::XMFLOAT3 &r_look_at);
	Camera(const float pos_x,
		const float pos_y,
		const float pos_z,
		const float up_x,
		const float up_y,
		const float up_z,
		const float look_x,
		const float look_y,
		const float look_z);

	~Camera();

	void SetCameraPosition(const DirectX::XMFLOAT3 &r_new_position);
	void SetCameraPosition(const DirectX::XMVECTOR &r_new_position);
	void SetCameraPosition(const float new_x, const float new_y, const float new_z);

	void SetUpVector(const DirectX::XMFLOAT3 &r_new_up);
	void SetUpVector(const DirectX::XMVECTOR &r_new_up);
	void SetUpVector(const float new_x, const float new_y, const float new_z);

	void SetLookAt(const DirectX::XMFLOAT3 &r_new_look_at);
	void SetLookAt(const DirectX::XMVECTOR &r_new_look_at);
	void SetLookAt(const float new_x, const float new_y, const float new_z);

	void RotatePitchYawRoll(
		const float pitch,
		const float yaw,
		const float roll);
	void RotatePitchYawRoll(DirectX::XMFLOAT3 &pitch_yaw_roll);
	void RotatePitchYawRoll(DirectX::XMVECTOR &pitch_yaw_roll);

	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetUpVector() const;
	DirectX::XMVECTOR GetLookAt() const;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetTransposedViewMatrix() const;

private:
	DirectX::XMVECTOR mCameraPosition;
	DirectX::XMVECTOR mUpVector;
	DirectX::XMVECTOR mLookAt;
	DirectX::XMMATRIX mViewMatrix;
	void mInit(const DirectX::XMVECTOR & r_position,
		const DirectX::XMVECTOR & r_up_vector,
		const DirectX::XMVECTOR & r_look_at);
	void mUpdateViewMatrix();
	void mRotateViewMatrix(const DirectX::XMMATRIX &camRotationMatrix); 
	 //! IF SOMETHING IS BROKEN WITH CAMERA ROTATION
	 //! mRotateViewMatrix IS PROBABLY THE ROOT CAUSE
	 
};