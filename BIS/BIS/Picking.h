#pragma once
#include <DirectXMath.h>

namespace Picking
{
	void GetWorldRay
	(
		const DirectX::XMMATRIX &projectionMatrix,
		const DirectX::XMMATRIX &viewMatrix,
		float nScreenX,
		float nScreenY,
		DirectX::XMVECTOR &rRayOrigin,
		DirectX::XMVECTOR &rRayDirection);
}