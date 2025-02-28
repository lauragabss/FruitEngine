#pragma once

#include "../ECS/ECS.h"
#include <SDL3/SDL.h>

class RenderColliderSystem : public System
{
public:
	RenderColliderSystem();

	void Update(SDL_Renderer* renderer);
};
