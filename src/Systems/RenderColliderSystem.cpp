#include "RenderColliderSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

RenderColliderSystem::RenderColliderSystem()
{
	RequireComponent<BoxColliderComponent>();
	RequireComponent<TransformComponent>();
}

void RenderColliderSystem::Update(SDL_Renderer* renderer)
{
	for (auto entity: GetSystemEntities())
	{
		const auto transform = entity.GetComponent<TransformComponent>();
		const auto collider = entity.GetComponent<BoxColliderComponent>();

		SDL_FRect colliderRect = {
			(transform.position.x + collider.offset.x),
			(transform.position.y + collider.offset.y),
			(collider.width),
			(collider.height)
		};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderRect(renderer, &colliderRect);
	}
}
