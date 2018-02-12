#include "Picking.h"
#include <algorithm>
#include <fstream>

void Picking::GetWorldRay(
	const Camera *pCamera,
	float nScreenX,
	float nScreenY,
	Ray &rRay)
{
	//float width, height, nearZ;

	//// Map click location from [0.0;1.0] to [-1.0;1.0]
	//nScreenX = nScreenX * 2.f - 1.f;
	//nScreenY = (nScreenY * 2.f - 1.f) * (-1.f);	// Invert "up" direction

	//// Get resourses from camera
	//width = pCamera->GetViewWidth();
	//height = pCamera->GetViewHeight();
	//nearZ = pCamera->GetNearZ();

	///**
	//*	Calculate ray origin
	//*		The pick is happening on the screen, so the ray should start from the near
	//*		plane and not from the cameras position.
	//*/

	//DirectX::XMVECTOR frontVec, rightVec;

	//// 1. Walk to nearplane
	//frontVec = DirectX::XMVectorScale(
	//	pCamera->GetLookToVector(),
	//	nearZ);

	//rRay.origin = DirectX::XMVectorAdd(
	//	pCamera->GetPosition(),
	//	frontVec);

	//// 2. Walk left/right
	//rightVec = DirectX::XMVector3Cross(
	//	pCamera->GetUpVector(),
	//	pCamera->GetLookToVector());

	//rRay.origin = DirectX::XMVectorAdd(
	//	rRay.origin,
	//	DirectX::XMVectorScale(rightVec, nScreenX * (width/2.f)));

	//// 3. Walk up/down
	//rRay.origin = DirectX::XMVectorAdd(
	//	rRay.origin,
	//	DirectX::XMVectorScale(pCamera->GetUpVector(), nScreenY * (height/2.f)));

	///**
	//*	The ray origin is now positioned correctly on the nearplane (screen).
	//*	Now, we need to calculate the direction of the ray, depending on the
	//* cameras settings.
	//*
	//*	Calculate direction of ray
	//*		See the nearplane as the screen. We know where on the screen the user
	//*		has the cursor (nScreenX and nScreenY), so all we have to do is go to
	//*		the corresponding point on the nearplane and calculating the direction
	//*		from the position of the eye (Perspective), or the direction of the
	//*		camera (orthografic).
	//*/

	//if (pCamera->GetProjectionMode() == PERSPECTIVE)
	//{
	//	rRay.direction = DirectX::XMVectorSubtract(
	//		rRay.origin,
	//		pCamera->GetPosition());
	//}
	//else // ORTHOGRAFIC
	//{
	//	rRay.direction = pCamera->GetLookToVector();
	//}

	//// Normalize the direction of the ray
	//rRay.direction = DirectX::XMVector3Normalize(rRay.direction);

	if (pCamera->GetProjectionMode() == PROJECTION_MODE::PERSPECTIVE)
	{
		DirectX::XMFLOAT4X4 fProjectionMatrix;

		float width, height;
		float xView, yView;

		//nScreenX = nScreenX * 2.f - 1.f;
		//nScreenY = nScreenY * 2.f - 1.f;

		//// Get resourses from camera
		width = pCamera->GetViewWidth();
		height = pCamera->GetViewHeight();

		// Get a float-matrix of projection
		DirectX::XMStoreFloat4x4(&fProjectionMatrix, pCamera->GetProjectionMatrix());

		//xView = (width/height) * ((2.f * nScreenX / width) - 1);
		//yView = (-2.f * nScreenY / height) + 1;
		xView = ((+2.f * nScreenX) - 1.0f) / fProjectionMatrix._11;
		yView = ((-2.f * nScreenY) + 1.0f) / fProjectionMatrix._22;

		DirectX::XMVECTOR rayOrigin = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR rayDirection = { xView, yView, 1.0f, 0.0f };

		DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(NULL, pCamera->GetViewMatrix());
		rayOrigin = DirectX::XMVector4Transform(rayOrigin, invView);
		rayDirection = DirectX::XMVector4Transform(rayDirection, invView);

		rRay.origin = rayOrigin;
		rRay.direction = DirectX::XMVector3Normalize(rayDirection);
	}
	else // ORTHOGRAPHIC
	{
		nScreenX = nScreenX * 2.f - 1.f;
		nScreenY = -(nScreenY * 2.f - 1.f);	// Invert "up" direction

		// Get resourses from camera
		float width = pCamera->GetViewWidth();
		float height = pCamera->GetViewHeight();
		float nearZ = pCamera->GetNearZ();

		/**
		*	Calculate ray origin
		*		The pick is happening on the screen, so the ray should start from the near
		*		plane and not from the cameras position.
		*/

		DirectX::XMVECTOR frontVec, rightVec;

		// 1. Walk to nearplane
		frontVec = DirectX::XMVectorScale(
			pCamera->GetLookToVector(),
			nearZ);

		rRay.origin = DirectX::XMVectorAdd(
			pCamera->GetPosition(),
			frontVec);

		// 2. Walk left/right
		rightVec = DirectX::XMVector3Cross(
			pCamera->GetUpVector(),
			pCamera->GetLookToVector());

		rRay.origin = DirectX::XMVectorAdd(
			rRay.origin,
			DirectX::XMVectorScale(rightVec, nScreenX * (width/2.f)));

		// 3. Walk up/down
		rRay.origin = DirectX::XMVectorAdd(
			rRay.origin,
			DirectX::XMVectorScale(pCamera->GetUpVector(), nScreenY * (height/2.f)));

		rRay.direction = pCamera->GetLookToVector();
	}
		
}

float Picking::IsRayIntersectingAABB(
	const Ray &ray,
	const AABB &box)
{
	// Initializing variables
	float tmin = -1.0f;
	float tmax = 100000.f;
	float t1, t2, factor;

	// Storing vectors as floats instead 
	float fOrigin[3] = {
		DirectX::XMVectorGetX(ray.origin),
		DirectX::XMVectorGetY(ray.origin),
		DirectX::XMVectorGetZ(ray.origin)
	};
	float fDirection[3] = {
		DirectX::XMVectorGetX(ray.direction),
		DirectX::XMVectorGetY(ray.direction),
		DirectX::XMVectorGetZ(ray.direction)
	};

	AABB::Extrema slabs[3] = {
		box.x,
		box.y,
		box.z
	};
	for (unsigned int i = 0; i < 3; i++)
	{
		if (std::fabs(fDirection[i]) < 0.000001f)
		{
			// Early exit
			if (fOrigin[i] < slabs[i].min || fOrigin[i] > slabs[i].max)
				return -1.0f;
		}
		else
		{
			factor = 1.0f / fDirection[i];
			t1 = (slabs[i].min - fOrigin[i]) * factor;
			t2 = (slabs[i].max - fOrigin[i]) * factor;

			if (t1 > t2)
				std::swap(t1, t2);

			tmin = (tmin < t1) ? t1 : tmin;
			tmax = (tmax > t2) ? t2 : tmax;

			if (tmin > tmax)
				return -1.0f;
		}
	}

	return tmin;
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
