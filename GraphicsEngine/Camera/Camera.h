#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

enum LOOKMODE {
	LOOKAT = 0,
	LOOKTO = 1
};

enum PROJECTIONMODE {
	PERSPECTIVE = 0,
	ORTHOGRAPHIC = 1
};
/*
Camera contains everything you (should) need regarding view and projection matrices.
LOOKMODE determines if you want the look vector to represent look AT or look TO.
PROJECTIONMODE sets if you want to use perspective of orthographic. Keep in mind that
after swapping you have to set the related variables for the type you are swapping to,
or else you are gonna have a bad time.
*/

class Camera {
public:
	Camera(const DirectX::XMVECTOR &r_position,
		const DirectX::XMVECTOR &r_up_vector,
		const DirectX::XMVECTOR &r_look_vector,
		const float view_width_or_fov_angle,
		const float view_height_or_aspect_ratio,
		const float near_z,
		const float far_z,
		const LOOKMODE camera_mode = LOOKAT,
		const PROJECTIONMODE projection_mode = PERSPECTIVE);

	Camera(const DirectX::XMFLOAT3 &r_position,
		const DirectX::XMFLOAT3 &r_up_vector,
		const DirectX::XMFLOAT3 &r_look_vector,
		const float view_width_or_fov_angle,
		const float view_height_or_aspect_ratio,
		const float near_z,
		const float far_z,
		const LOOKMODE camera_mode = LOOKAT,
		const PROJECTIONMODE projection_mode = PERSPECTIVE);

	Camera(const float pos_x,
		const float pos_y,
		const float pos_z,
		const float up_x,
		const float up_y,
		const float up_z,
		const float look_x,
		const float look_y,
		const float look_z,
		const float view_width_or_fov_angle,
		const float view_height_or_aspect_ratio,
		const float near_z,
		const float far_z,
		const LOOKMODE camera_mode = LOOKAT,
		const PROJECTIONMODE projection_mode = PERSPECTIVE);

	~Camera();

	void SetCameraPosition(const DirectX::XMFLOAT3 &r_new_position);
	void SetCameraPosition(const DirectX::XMVECTOR &r_new_position);
	void SetCameraPosition(const float new_x,
		const float new_y,
		const float new_z);

	// MoveCamera takes a direction vector and a magnitude to move along that vector.
	// The function does not normalize the vector, so if the vector is wonky that's
	// on you.

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

	void SetLookMode(const LOOKMODE new_look_mode);

	void RotateCameraPitchYawRoll(
		const float pitch,
		const float yaw,
		const float roll);
	void RotateCameraPitchYawRoll(const DirectX::XMFLOAT3 &pitch_yaw_roll);
	void RotateCameraPitchYawRoll(const DirectX::XMVECTOR &pitch_yaw_roll);

	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetUpVector() const;
	DirectX::XMVECTOR GetLookVector() const;
	LOOKMODE GetLookMode() const;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetTransposedViewMatrix() const;

	DirectX::XMMATRIX GetProjectionMatrix() const;
	DirectX::XMMATRIX GetTransposedProjectionMatrix() const;
	PROJECTIONMODE GetProjectionMode() const;

	bool SetViewWidth(const float new_view_width); // Not used in PERSPECTIVE mode
	bool SetViewHeight(const float new_view_height); // Not used in PERSPECTIVE mode
	bool SetFovAngle(const float new_fov_angle); // Not used in ORTHOGRAPHIC mode
	bool SetAspectRatio(const float new_aspect_ratio); // Not used in ORTHOGRAPHIC mode
	bool SetNearZ(const float new_near_z);
	bool SetFarZ(const float new_far_z);

	void SetProjectionMode(const PROJECTIONMODE new_projection_mode);
	

private:
	DirectX::XMVECTOR mCameraPosition;
	DirectX::XMVECTOR mUpVector;
	DirectX::XMVECTOR mLookVector;

	float mViewWidth; // Not used in PERSPECTIVE mode
	float mViewHeight; // Not used in PERSPECTIVE mode
	float mFovAngle; // Not used in ORTHOGRAPHIC mode
	float mAspectRatio; // Not used in ORTHOGRAPHIC mode
	float mNearZ;
	float mFarZ;
	LOOKMODE mLookMode;
	PROJECTIONMODE mProjectionMode;
	DirectX::XMMATRIX mViewMatrix;
	DirectX::XMMATRIX mProjMatrix;


	void mInit(const DirectX::XMVECTOR &r_position,
		const DirectX::XMVECTOR &r_up_vector,
		const DirectX::XMVECTOR &r_look_vector,
		const float view_width_or_fov_angle,
		const float view_height_or_aspect_ratio,
		const float near_z,
		const float far_z,
		const LOOKMODE camera_mode,
		const PROJECTIONMODE projection_mode);
	void mUpdateViewMatrix();
	void mUpdateProjMatrix();
	void mRotateViewMatrix(const DirectX::XMMATRIX &camRotationMatrix); 
	 //! IF SOMETHING IS BROKEN WITH CAMERA ROTATION
	 //! mRotateViewMatrix IS PROBABLY THE ROOT CAUSE
	 
};