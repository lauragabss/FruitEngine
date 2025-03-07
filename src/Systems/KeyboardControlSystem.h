#pragma once
#include "../ECS/ECS.h"

class EventBus;
class KeyPressedEvent;

class KeyboardControlSystem : public System
{
public:
	KeyboardControlSystem();

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus);

	void OnKeyPressed(KeyPressedEvent& event);

	void Update();
};
