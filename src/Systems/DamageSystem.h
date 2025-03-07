#pragma once
#include "../ECS/ECS.h"
#include <memory>

class EventBus;
class CollisionEvent;

class DamageSystem: public System 
{
public:
	DamageSystem();

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);

	void OnCollision(CollisionEvent& event);

	void Update();
};
