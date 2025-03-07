#include "AnimationSystem.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL3/SDL.h>

AnimationSystem::AnimationSystem()
{
	RequireComponent<AnimationComponent>();
	RequireComponent<SpriteComponent>();
}

void AnimationSystem::Update()
{
	for (auto entity: GetSystemEntities())
	{
		auto& animationComponent = entity.GetComponent<AnimationComponent>();
		auto& spriteComponent = entity.GetComponent<SpriteComponent>();

		animationComponent.currentFrame = (
			(SDL_GetTicks() - animationComponent.startTime)
			* animationComponent.frameSpeedRate / 1000) % animationComponent.numFrames;

		if (animationComponent.animDirection == horizontal) 
		{
		spriteComponent.sourceRec.x = animationComponent.currentFrame * spriteComponent.width;
		}
		else 
		{
			spriteComponent.sourceRec.y = animationComponent.currentFrame * spriteComponent.height;
		}
	}
}
