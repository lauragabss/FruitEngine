#include "DamageSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

DamageSystem::DamageSystem()
{
	RequireComponent<BoxColliderComponent>();
}

void DamageSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
	eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
}

void DamageSystem::OnCollision(CollisionEvent& event)
{
	Logger::Log("Damage system received an EVENT COLLISION!!!");
	//event.a.Kill();
	//event.b.Kill();
}

void DamageSystem::Update()
{
}
