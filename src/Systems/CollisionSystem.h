#pragma once

#include "../ECS/ECS.h"

class CollisionSystem: public System 
{
public:
	CollisionSystem();

	void Update();

	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH);
};
