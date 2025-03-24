#pragma once
#include "../ECS/ECS.h"

class ProjectileEmitSystem : public System
{
public:
	ProjectileEmitSystem();

	void Update(Registry& registry);
};
