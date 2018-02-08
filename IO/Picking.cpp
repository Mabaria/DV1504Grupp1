#include "Picking.h"
#include <algorithm>
#include <fstream>

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
	*	Calculate ray origin
	*		The pick is happening on the screen, so the ray should start from the near
	*		plane and not from the cameras position.
	*/

	DirectX::XMVECTOR frontVec, rightVec;

	// 1. Walk to nearplane
	frontVec = DirectX::XMVectorScale(
		pCamera->GetLookVector(),
		nearZ);

	rRay.origin = DirectX::XMVectorAdd(
		pCamera->GetPosition(),
		frontVec);

	// 2. Walk left/right
	rightVec = DirectX::XMVector3Cross(
		pCamera->GetUpVector(),
		pCamera->GetLookVector());

	rRay.origin = DirectX::XMVectorAdd(
		rRay.origin,
		DirectX::XMVectorScale(rightVec, nScreenX * (width/2.f)));

	// 3. Walk up/down
	rRay.origin = DirectX::XMVectorAdd(
		rRay.origin,
		DirectX::XMVectorScale(pCamera->GetUpVector(), nScreenY * (height/2.f)));

	/**
	*	The ray origin is now positioned correctly on the nearplane (screen).
	*	Now, we need to calculate the direction of the ray, depending on the
	* cameras settings.
	*
	*	Calculate direction of ray
	*		See the nearplane as the screen. We know where on the screen the user
	*		has the cursor (nScreenX and nScreenY), so all we have to do is go to
	*		the corresponding point on the nearplane and calculating the direction
	*		from the position of the eye (Perspective), or the direction of the
	*		camera (orthografic).
	*/

	if (pCamera->GetProjectionMode() == PERSPECTIVE)
	{
		rRay.direction = DirectX::XMVectorSubtract(
			rRay.origin,
			pCamera->GetPosition());
	}
	else // ORTHOGRAFIC
	{
		rRay.direction = pCamera->GetLookVector();
	}

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
