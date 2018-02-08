#pragma once
#include "../GraphicsEngine/Camera/Camera.h"
#include "Picking.h"
#include <cmath>
#include <math.h>
#include <iostream>

void Test_Picking();
void Test_Picking_ClickToRay_Perspective();
void Test_Picking_ClickToRay_Orthographic();

float DotProduct(DirectX::XMVECTOR vec1, DirectX::XMVECTOR vec2);

// Help functions for Click To Ray testing
bool CheckRay_Orthographic(
	Ray ray,
	DirectX::XMVECTOR pos,
	DirectX::XMVECTOR dir);
DirectX::XMVECTOR ComputePosition(Camera *pCamera, float posX, float posY);
