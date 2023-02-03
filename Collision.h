#pragma once

#include "CollisionPrimitive.h"

using namespace DirectX;

class Collision
{
public:
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane&
		plane, DirectX::XMVECTOR* inter = nullptr);
};

