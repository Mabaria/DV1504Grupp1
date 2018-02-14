#include "Picking.h"
#include <algorithm>
#include <fstream>

void Picking::GetWorldRay(
	const Camera *pCamera,
	float nScreenX,
	float nScreenY,
	Ray &rRay)
{

	/**
	*	GetWorldRay is split into two modes, depending on the cameras projection
	*	mode.
	*/

	/**
	*	Perspective camera
	*		Since the click happens in Screen space, we need to map the ray from
	*		Screen space to World space in this function. The steps between will be:
	*
	*		Screen space -> NDC -> Projection space -> View space -> World space
	*										
	*/

	if (pCamera->GetProjectionMode() == PROJECTION_MODE::PERSPECTIVE)
	{
		DirectX::XMFLOAT4X4 fProjectionMatrix;
		float width, height, xView, yView;

		//// Get resourses from camera
		width = pCamera->GetViewWidth();
		height = pCamera->GetViewHeight();

		// Get a float-matrix of projection
		DirectX::XMStoreFloat4x4(&fProjectionMatrix, pCamera->GetProjectionMatrix());

		//	The steps (Screen -> NDC -> Proj -> View) combined
		xView = ((+2.f * nScreenX) - 1.0f) / fProjectionMatrix._11;
		yView = ((-2.f * nScreenY) + 1.0f) / fProjectionMatrix._22;

		// Create ray origin and direction in view space
		DirectX::XMVECTOR rayOrigin = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR rayDirection = { xView, yView, 1.0f, 0.0f };

		// Map ray to World space using the inverse of the view matrix
		DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(
			NULL,
			pCamera->GetViewMatrix());
		rayOrigin = DirectX::XMVector4Transform(rayOrigin, invView);
		rayDirection = DirectX::XMVector4Transform(rayDirection, invView);

		// Set the values to the returning ray
		rRay.origin = rayOrigin;
		rRay.direction = DirectX::XMVector3Normalize(rayDirection);
	}



	else // ORTHOGRAPHIC
	{
		// Map x and y from [0;1] to [-1;1]
		nScreenX = nScreenX * 2.f - 1.f;
		nScreenY = -(nScreenY * 2.f - 1.f);	// Invert "up" direction

		// Get resourses from camera
		float width = pCamera->GetViewWidth();
		float height = pCamera->GetViewHeight();
		float nearZ = pCamera->GetNearZ();

		/**
		*	Calculate ray origin
		*		The pick is happening on the screen, so the ray should start
		*		from the near plane and not from the cameras position.
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

		// Give the ray the same direction as the camera
		rRay.direction = pCamera->GetLookToVector();
	}
}

float Picking::IsRayIntersectingAABB(
	const Ray &ray,
	const AABB &box)
{
	// Initializing variables
	float tmin = -100000.0f;
	float tmax = 100000.f;
	float t1, t2, factor;

	/**
	*	Create arrays for easy looping
	*/

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
		// Sanity check ray parallel
		if (std::fabs(fDirection[i]) < 0.000001f)
		{
			/**
			* If parallel, the ray will never touch the walls of the slab.
			* In this case, we check if the ray position is within the walls.
			*/

			if (fOrigin[i] < slabs[i].min || fOrigin[i] > slabs[i].max)
				return -1.0f;
		}
		else
		{
			factor = 1.0f / fDirection[i];
			t1 = (slabs[i].min - fOrigin[i]) * factor; // Slab entry
			t2 = (slabs[i].max - fOrigin[i]) * factor; // Slab exit

			if (t1 > t2) // If working with negative values
				std::swap(t1, t2);

			// Store the highest tmin, and the lowest tmax
			tmin = (tmin < t1) ? t1 : tmin;
			tmax = (tmax > t2) ? t2 : tmax;

			// If at any time the tmin is higher than tmax, we found a miss
			if (tmin > tmax)
				return -1.0f;
		}
	}

	return tmin;
}

AABB Picking::FromVerticesToAABB(const std::vector<Vertex> &vertex)
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