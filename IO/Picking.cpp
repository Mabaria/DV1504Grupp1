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
	DirectX::XMMATRIX inverseProjection, inverseView;
	DirectX::XMFLOAT4X4 fInverseView;

	// Making sure screen positions are between -1.0 and 1.0
	nScreenX = nScreenX * 2.0f - 1.0f;
	nScreenY = nScreenY * 2.0f - 1.0f;

	// TODO: Is this correct? http://www.rastertek.com/dx11tut47.html
	// Fits screen coordinates to viewport
	nScreenX /= DirectX::XMVectorGetX(projectionMatrix.r[0]);
	nScreenY /= DirectX::XMVectorGetY(projectionMatrix.r[1]);


	// Inversing the matrices
	inverseProjection =
		DirectX::XMMatrixInverse(nullptr, projectionMatrix);
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
		0.0f	// Should maybe be 1.0f
	};
	return;
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
