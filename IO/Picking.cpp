#include "Picking.h"
#include <algorithm>
#include <fstream>

void Picking::GetWorldRay(
	const DirectX::XMMATRIX &projectionMatrix,
	const DirectX::XMMATRIX &viewMatrix,
	float nScreenX,
	float nScreenY,
	Ray &rRay)
{
	// Declaring variables used throughout
	DirectX::XMMATRIX inverseView;
	DirectX::XMFLOAT4X4 fInverseView;

	// Making sure screen positions are between -1.0 and 1.0
	nScreenX = nScreenX * 2.0f - 1.0f;
	nScreenY = nScreenY * 2.0f - 1.0f;

	// EDIT --------------------------
	DirectX::XMFLOAT4X4 fProjectionMatrix;

	// Get a float-matrix of projection
	DirectX::XMStoreFloat4x4(&fProjectionMatrix, projectionMatrix);
	// --------------------------

	// TODO: Is this correct? http://www.rastertek.com/dx11tut47.html
	// Fits screen coordinates to viewport
	//nScreenX /= DirectX::XMVectorGetX(projectionMatrix.r[0]);
	//nScreenY /= DirectX::XMVectorGetY(projectionMatrix.r[1]);
	nScreenX /= fProjectionMatrix._11;
	nScreenY /= fProjectionMatrix._22;


	// Inversing the matrice
	inverseView =
		DirectX::XMMatrixInverse(nullptr, viewMatrix);
	
	// Get float data from inverse view matrix
	DirectX::XMStoreFloat4x4(&fInverseView, inverseView);

	// Extract camera position from view Matrix
	rRay.origin = inverseView.r[3];

	// Extract a direction from the view matrix
	rRay.direction = {
		// x
		(nScreenX * fInverseView._11) +
		(nScreenY * fInverseView._21) +
		fInverseView._31,
		// y
		(nScreenX * fInverseView._12) +
		(nScreenY * fInverseView._22) +
		fInverseView._32,
		// z
		(nScreenX * fInverseView._13) +
		(nScreenY * fInverseView._23) +
		fInverseView._33,
		// w
		0.0f
	};

	rRay.direction = DirectX::XMVector4Normalize(rRay.direction);

	return;
}

//void Picking::GetWorldRay(
//	const Camera *pCamera,
//	float nScreenX,
//	float nScreenY,
//	Ray &rRay)
//{
//	// Note: Following picking method can be found in
//	// Introductoction to 3D Game Programming with DirectX 10, s384
//	// (Link can be found in 3D lecture about picking and shadowmapping on It's)
//
//	/**
//	*	Step 1: Create ray in view space
//	*/
//
//	// Declaring variables used throughout
//	DirectX::XMFLOAT4X4 fProjectionMatrix;
//	float screenWidth, screenHeight;
//
//	// Get a float-matrix of projection
//	DirectX::XMStoreFloat4x4(&fProjectionMatrix, pCamera->GetProjectionMatrix());
//
//	// Extract screen width and height from the projection matrix
//	screenWidth = pCamera->GetViewWidth();
//	screenHeight = pCamera->GetViewHeight();
//
//	// Convert to view space coordinates
//	float xView = (( 2.f * nScreenX) / screenWidth - 1) / fProjectionMatrix._11;
//	float yView = ((-2.f * nScreenY) / screenHeight + 1) / fProjectionMatrix._22;
//
//	// Set ray in view space..
//	// (Note: The "eye-position" is origo in view space)
//	rRay.origin = {0.f, 0.f, 0.f, 0.f};
//	rRay.direction = {xView, yView, 1.f, 1.f};
//
//
//
//	/**
//	*	Step 2: Convert ray from view space to world space
//	*/
//
//	// ..convert ray to world space with the inverse of view matrix
//	DirectX::XMMATRIX viewMatrixInverse;
//	viewMatrixInverse = DirectX::XMMatrixInverse(nullptr, pCamera->GetViewMatrix());
//
//	// (May need to change order of operations)
//	rRay.origin = DirectX::XMVector4Transform(rRay.origin, viewMatrixInverse);
//	rRay.direction = DirectX::XMVector4Transform(rRay.direction, viewMatrixInverse);
//	
//	return;
//}

void Picking::GetWorldRay(
	const Camera *pCamera,
	float nScreenX,
	float nScreenY,
	Ray &rRay)
{
	float width, height, nearZ;

	nScreenX = nScreenX * 2.f - 1.f;
	nScreenY = (nScreenY * 2.f - 1.f) * (-1.f);

	width = pCamera->GetViewWidth();
	height = pCamera->GetViewHeight();
	nearZ = pCamera->GetNearZ();

	/**
	*	Calculate direction of ray
	*		See the nearplane as the screen. We know where on the screen the user
	*		has the cursor (nScreenX and nScreenY), so all we have to do is go to
	*		the corresponding point on the nearplane and calculating the direction
	*		from the position of the eye (camera).
	*/

	DirectX::XMVECTOR frontVec, rightVec;

	// 1. Walk to nearplane
	frontVec = DirectX::XMVectorScale(
		pCamera->GetLookVector(),
		nearZ);

	rRay.direction = frontVec;

	// 2. Walk left/right
	rightVec = DirectX::XMVector3Cross(
		pCamera->GetUpVector(),
		pCamera->GetLookVector());

	rRay.direction = DirectX::XMVectorAdd(
		rRay.direction,
		DirectX::XMVectorScale(rightVec, nScreenX * (width/2.f)));

	// 3. Walk up/down
	rRay.direction = DirectX::XMVectorAdd(
		rRay.direction,
		DirectX::XMVectorScale(pCamera->GetUpVector(), nScreenY * (height/2.f)));

	/**
	*	Calculate ray origin
	*		The pick is happening on the screen, so the ray should start from the near
	*		plane and not from the cameras position.
	*/

	rRay.origin = DirectX::XMVectorAdd(
		pCamera->GetPosition(),
		rRay.direction);
	
	// Normalize the direction of the ray
	rRay.direction = DirectX::XMVector4Normalize(rRay.direction);
}

bool Picking::IsRayIntersectingAABB(
	const Ray &ray,
	const AABB &box)
{
	// Initializing variables
	DirectX::XMFLOAT3 fRayOrigin, fRayDirection;
	float tmin, tymin, tzmin;
	float tmax, tymax, tzmax;

	// Storing vectors as floats instead
	DirectX::XMStoreFloat3(&fRayOrigin, ray.origin);
	DirectX::XMStoreFloat3(&fRayDirection, ray.direction);

	// Sanity check this
	// if (fRayDirection.x > 0.0001f)
	tmin = (box.x.min - fRayOrigin.x) / fRayDirection.x;
	tmax = (box.x.max - fRayOrigin.x) / fRayDirection.x;

	if (tmin > tmax)
		std::swap(tmin, tmax);

	tymin = (box.y.min - fRayOrigin.y) / fRayDirection.y;
	tymax = (box.y.max - fRayOrigin.y) / fRayDirection.y;

	if (tymin > tymax)
		std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	tzmin = (box.z.min - fRayOrigin.z) / fRayDirection.z;
	tzmax = (box.z.max - fRayOrigin.z) / fRayDirection.z;

	if (tzmin > tzmax)
		std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}

bool FillAABBVectorFromFile(const std::string &path, std::vector<AABB> &rList)
{
	AABB *pList;
	int listSize = 0;

	std::ifstream infile(path, std::ifstream::binary);
	if (!infile.is_open())
		return false;

	infile.read((char*)&listSize, sizeof(int));
	pList = new AABB[listSize];

	infile.read((char*)pList, sizeof(AABB)*listSize);

	for (int i = 0; i < listSize; i++)
	{
		rList.push_back(pList[i]);
	}

	delete[] pList;
	return true;
}
