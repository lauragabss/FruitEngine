#include "CameraMovementSystem.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../Game/Game.h"

CameraMovementSystem::CameraMovementSystem()
{
	RequireComponent<CameraFollowComponent>();
	RequireComponent<TransformComponent>();
}

void CameraMovementSystem::Update(SDL_Rect& camera)
{
	for (auto entity: GetSystemEntities())
	{
		auto transform = entity.GetComponent<TransformComponent>();

		if (transform.position.x + (camera.w / 2) < Game::MapWidth)
		{
			camera.x = transform.position.x - (camera.w/2);
		}
		
		if (transform.position.y + (camera.h / 2) < Game::MapHeight)
		{
			camera.y = transform.position.y - (camera.h/2);
		}

		// Keep camera rectangle view inside screen limits
		camera.x = (camera.x < 0) ? 0 : camera.x;

		camera.y = (camera.y < 0) ? 0 : camera.y;
	}
}
