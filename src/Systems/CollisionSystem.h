#pragma once

#include "../ECS/ECS.h"

class EventBus;

class CollisionSystem: public System 
{
public:
	CollisionSystem();

	void Update(std::unique_ptr<EventBus>& eventBus);

	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH);
};
