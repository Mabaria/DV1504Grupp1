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

	// Map click location from [0.0;1.0] to [-1.0;1.0]
	nScreenX = nScreenX * 2.f - 1.f;
	nScreenY = (nScreenY * 2.f - 1.f) * (-1.f);	// Invert "up" direction

	// Get resourses from camera
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

float Picking::IsRayIntersectingAABB(
	const Ray &ray,
	const AABB &box)
{
	// Initializing variables
	DirectX::XMFLOAT3 fRayOrigin, fRayDirection;
	float tXMin, tYMin, tZMin;
	float tXMax, tYMax, tZMax;

	// Storing vectors as floats instead
	DirectX::XMStoreFloat3(&fRayOrigin, ray.origin);
	DirectX::XMStoreFloat3(&fRayDirection, ray.direction);

	tXMin = box.x.min - fRayOrigin.x;
	tXMax = box.x.max - fRayOrigin.x;

	// Sanity check
	if (fRayDirection.x != 0.f)
	{
			tXMin /= fRayDirection.x;
			tXMax /= fRayDirection.x;
	}

	if (tXMin > tXMax)
		std::swap(tXMin, tXMax);

	tYMin = box.y.min - fRayOrigin.y;
	tYMax = box.y.max - fRayOrigin.y;

	// Sanity check
	if (fRayDirection.y != 0.f)
	{
		tYMin /= fRayDirection.y;
		tYMax /= fRayDirection.y;
	}

	if (tYMin > tYMax)
		std::swap(tYMin, tYMax);

	if ((tXMin > tYMax) || (tYMin > tXMax))
		return -1.f;

	if (tYMin > tXMin)
		tXMin = tYMin;

	if (tYMax < tXMax)
		tXMax = tYMax;

	tZMin = box.z.min - fRayOrigin.z;
	tZMax = box.z.max - fRayOrigin.z;

	// Sanity check
	if (fRayDirection.z != 0.f)
	{
		tZMin /= fRayDirection.z;
		tZMax /= fRayDirection.z;
	}

	if (tZMin > tZMax)
		std::swap(tZMin, tZMax);

	if ((tXMin > tZMax) || (tZMin > tXMax))
		return -1.f;


	/**
	*	Compute the length (t) from ray origin to point of collision
	*/

	// 1. Create a vector of all tMins
	DirectX::XMVECTOR vec = {tXMin, tYMin, tZMin};
	
	// 2. Get length of vec (result will be stored in all components of vec)
	vec = DirectX::XMVector3Length(vec);

	// 3. Return length of vector
	return DirectX::XMVectorGetX(vec);
}

AABB Picking::FromMeshToAABB(const std::vector<Vertex> &vertex)
{
	AABB retAABB;
	retAABB.x.max = retAABB.x.min = vertex[0].x;
	retAABB.y.max = retAABB.y.min = vertex[0].y;
	retAABB.z.max = retAABB.z.min = vertex[0].z;

	for (unsigned int i = 1; i < vertex.size(); i++)
	{
		if (retAABB.x.max < vertex[i].x)
			retAABB.x.max = vertex[i].x;
		else if (retAABB.x.min > vertex[i].x)
			retAABB.x.min = vertex[i].x;

		if (retAABB.y.max < vertex[i].y)
			retAABB.y.max = vertex[i].y;
		else if (retAABB.y.min > vertex[i].y)
			retAABB.y.min = vertex[i].y;

		if (retAABB.z.max < vertex[i].z)
			retAABB.z.max = vertex[i].z;
		else if (retAABB.z.min > vertex[i].z)
			retAABB.z.min = vertex[i].z;
	}
	return retAABB;
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
