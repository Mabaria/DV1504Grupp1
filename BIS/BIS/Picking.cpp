#include "Picking.h"

void Picking::GetWorldRay(
	const DirectX::XMMATRIX &projectionMatrix,
	const DirectX::XMMATRIX &viewMatrix,
	float nScreenX,
	float nScreenY,
	DirectX::XMVECTOR &rRayOrigin,
	DirectX::XMVECTOR &rRayDirection)
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
	rRayOrigin = inverseView.r[3];

	// Extract a direction from the view matrix
	rRayDirection = {
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