#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

struct ProjectileEmitterComponent 
{
	glm::vec2 ProjectileVelocity;
	int RepeatFrequency;
	int ProjectileDuration;
	int HitPercentDamage;
	bool IsFriendly;
	int LastEmissionTime;

	ProjectileEmitterComponent(glm::vec2 ProjectileVelocity = glm::vec2(0), int RepeatFrequency = 0, int ProjectileDuration = 10000, int HitPercentDamage = 10, bool IsFriendly = false) {
		this->ProjectileVelocity = ProjectileVelocity;
		this->RepeatFrequency = RepeatFrequency;
		this->ProjectileDuration = ProjectileDuration;
		this->HitPercentDamage = HitPercentDamage;
		this->IsFriendly = IsFriendly;
		this->LastEmissionTime = SDL_GetTicks();
	}
};
