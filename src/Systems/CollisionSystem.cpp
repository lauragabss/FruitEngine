#include "CollisionSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

CollisionSystem::CollisionSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<BoxColliderComponent>();
}

void CollisionSystem::Update()
{
	// Check all entities that have a collision component and see if they are colliding wih each other
	auto entities = GetSystemEntities();
	for (auto i = entities.begin(); i != entities.end(); i++) 
	{
		Entity a = *i;
		auto aTransform = a.GetComponent<TransformComponent>();
		auto aCollider = a.GetComponent<BoxColliderComponent>();
		for (auto j = i; j != entities.end(); j++)
		{
			Entity b = *j;
			if (a == b)
			{
				// Bypass if we are trying to test the same entity
				continue;
			}
			auto bTransform = b.GetComponent<TransformComponent>();
			auto bCollider = b.GetComponent<BoxColliderComponent>();

			// AABB collision check
			bool collisionHappened = CheckAABBCollision(
				aTransform.position.x + aCollider.offset.x,
				aTransform.position.y + aCollider.offset.y,
				aCollider.width,
				aCollider.height,
				bTransform.position.x + bCollider.offset.x,
				bTransform.position.y + bCollider.offset.y,
				bCollider.width,
				bCollider.height
			);

			if (collisionHappened)
			{
				Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with " + std::to_string(b.GetId()));
			}
		}
	}
}

bool CollisionSystem::CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
{
	return (
		aX < bX + bW &&
		aX + aW > bX &&
		aY < bY + bH &&
		aY + aH > bY
		);
}
