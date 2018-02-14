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

namespace Picking
{
	void GetWorldRay (
		const Camera *pCamera,
		float nScreenX,
		float nScreenY,
		Ray &rRay);

	float IsRayIntersectingAABB(
		const Ray &ray,
		const AABB &box);

	AABB FromVerticesToAABB(const std::vector<Vertex> &vertex);
}
