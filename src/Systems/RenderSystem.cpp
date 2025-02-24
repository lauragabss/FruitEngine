#include "RenderSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

RenderSystem::RenderSystem()
{
	RequireComponent<SpriteComponent>();
	RequireComponent<TransformComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer)
{
	// Update entity position according to velocity every frame of the application
	for (auto entity : GetSystemEntities())
	{
		const auto transform = entity.GetComponent<TransformComponent>();
		const auto sprite = entity.GetComponent<SpriteComponent>();

		SDL_FRect objRect = {
			static_cast<int>(transform.position.x),
			static_cast<int>(transform.position.y),
			sprite.width,
			sprite.height
		};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
		SDL_RenderFillRect(renderer, &objRect);
	}
}
