#include "ProjectileEmitSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

ProjectileEmitSystem::ProjectileEmitSystem()
{
	RequireComponent<ProjectileEmitterComponent>();
	RequireComponent<TransformComponent>();
}

void ProjectileEmitSystem::Update(Registry& registry)
{
	for (auto entity : GetSystemEntities())
	{
		auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		const auto transform = entity.GetComponent<TransformComponent>();

		if (SDL_GetTicks() - projectileEmitter.LastEmissionTime > projectileEmitter.RepeatFrequency)
		{
			glm::vec2 projectilePosition = transform.position;
			if(entity.HasComponent<SpriteComponent>())
			{
				const auto entitySprite = entity.GetComponent<SpriteComponent>();
				projectilePosition.x += (transform.scale.x * entitySprite.width) / 2;
				projectilePosition.y += (transform.scale.y * entitySprite.height) / 2;

			}

			// Add new projectile entity to registry
			Entity projectile = registry.CreateEntity();
			projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
			projectile.AddComponent<RigidBodyComponent>(projectileEmitter.ProjectileVelocity);
			projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
			projectile.AddComponent<BoxColliderComponent>(4, 4);

			// Update projectile emitter component last emission to current time
			projectileEmitter.LastEmissionTime = SDL_GetTicks();
		}
	}
}
