#include "KeyboardControlSystem.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

KeyboardControlSystem::KeyboardControlSystem()
{
	RequireComponent<KeyboardControlledComponent>();
	RequireComponent<SpriteComponent>();
	RequireComponent<RigidBodyComponent>();
}

void KeyboardControlSystem::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
{
	eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
}

void KeyboardControlSystem::OnKeyPressed(KeyPressedEvent& event)
{
	// Logger::Log("Keyboard event key pressed: " + std::string(SDL_GetKeyName(event.Keycode)));
	// Change spriteand velocity of entity
	for (auto entity: GetSystemEntities())
	{
		const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

		switch(event.Keycode)
		{
		case SDLK_UP:
			rigidbody.velocity = keyboardControl.UpVelocity;
			sprite.sourceRec.y = sprite.height * 0;
			break;
		case SDLK_RIGHT:
			rigidbody.velocity = keyboardControl.RightVelocity;
			sprite.sourceRec.y = sprite.height * 1;
			break;
		case SDLK_DOWN:
			rigidbody.velocity = keyboardControl.DownVelocity;
			sprite.sourceRec.y = sprite.height * 2;
			break;
		case SDLK_LEFT:
			rigidbody.velocity = keyboardControl.LeftVelocity;
			sprite.sourceRec.y = sprite.height * 3;
			break;
		}
	}
}

void KeyboardControlSystem::Update()
{
}
