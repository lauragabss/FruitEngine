#pragma once
#include <SDL3/SDL.h>
#include "../ECS/ECS.h"

class CameraMovementSystem: public System
{
public:
	CameraMovementSystem();

	void Update(SDL_Rect& camera);
};
