#include "MovementSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

MovementSystem::MovementSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<RigidBodyComponent>();
}

void MovementSystem::Update(double deltatime)
{
	// Update entity position according to velocity every frame of the application
	for (auto entity : GetSystemEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

		transform.position.x += rigidbody.velocity.x * deltatime;
		transform.position.y += rigidbody.velocity.y * deltatime;

		// Logger::Log("entity id = " + std::to_string(entity.GetId()) + "position = " + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y));
	}
}
