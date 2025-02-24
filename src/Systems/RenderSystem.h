#pragma once

#include <SDL3/SDL.h>
#include "../ECS/ECS.h"

class RenderSystem : public System
{
public:
	RenderSystem();

	void Update(SDL_Renderer* renderer);
};
