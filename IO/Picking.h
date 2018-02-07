#pragma once
#include <DirectXMath.h>
#include "../GraphicsEngine/Parsing/Mesh.h"
#include "../GraphicsEngine/Camera/Camera.h"

struct Ray {
	DirectX::XMVECTOR origin;
	DirectX::XMVECTOR direction;
};

struct AABB {
	struct Extrema {
		float min, max;
	};
	Extrema x, y, z;
};

//AABB FromIndexedMeshToAABB(
//	Mesh &mesh,
//	int index);

bool FillAABBVectorFromFile(const std::string &path, std::vector<AABB> &rList);

namespace Picking
{
	void GetWorldRay (
		const DirectX::XMMATRIX &projectionMatrix,
		const DirectX::XMMATRIX &viewMatrix,
		float nScreenX,
		float nScreenY,
		Ray &rRay);

	void GetWorldRay (
		const Camera *pCamera,
		float nScreenX,
		float nScreenY,
		Ray &rRay);

	bool IsRayIntersectingAABB(
		const Ray &ray,
		const AABB &box);
}
