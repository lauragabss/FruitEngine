#include "RenderSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../AssetManager/AssetManager.h"

RenderSystem::RenderSystem()
{
	RequireComponent<SpriteComponent>();
	RequireComponent<TransformComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, AssetManager& assetManager)
{
	// Update entity position according to velocity every frame of the application
	for (auto entity : GetSystemEntities())
	{
		const auto transform = entity.GetComponent<TransformComponent>();
		const auto sprite = entity.GetComponent<SpriteComponent>();

		// Section of texture to be used
		SDL_FRect sourceRec = sprite.sourceRec;

		// Rectangle position and size on screen
		SDL_FRect destinationRec = {
			transform.position.x,
			transform.position.y,
			sprite.width * transform.scale.x,
			sprite.height * transform.scale.y
		};

		SDL_RenderTextureRotated(
			renderer,
			assetManager.GetTexture(sprite.assetId),
			&sourceRec,
			&destinationRec,
			transform.rotation,
			NULL,
			SDL_FLIP_NONE
		);
	}
}
