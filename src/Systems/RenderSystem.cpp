#include "RenderSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../AssetManager/AssetManager.h"
#include <vector>
#include <algorithm>

RenderSystem::RenderSystem()
{
	RequireComponent<SpriteComponent>();
	RequireComponent<TransformComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, AssetManager& assetManager, SDL_Rect& camera)
{
	// Create a vector with sprite and transform components for all entities
	struct RenderableEntity {
		TransformComponent transformComponent;
		SpriteComponent spriteComponent;
	};
	std::vector<RenderableEntity> renderableEntities;

	for (auto entity : GetSystemEntities()) {
		RenderableEntity renderableEntity;
		renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
		renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
		renderableEntities.emplace_back(renderableEntity);
	}

	// WARNING: The performance of this can be improved if necessary
	// sort by z-order
	std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
		return a.spriteComponent.zindex < b.spriteComponent.zindex;
	});
	
	// Update entity position according to velocity every frame of the application
	for (auto entity :renderableEntities)
	{
		const auto transform = entity.transformComponent;
		const auto sprite = entity.spriteComponent;

		// Section of texture to be used
		SDL_FRect sourceRec = sprite.sourceRec;

		// Rectangle position and size on screen
		SDL_FRect destinationRec = {
			transform.position.x - camera.x,
			transform.position.y - camera.y,
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
