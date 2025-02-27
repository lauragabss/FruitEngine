#pragma once

#include <SDL3/SDL.h>

enum SpriteOrientation 
{
	horizontal = 0,
	vertical = 1
};

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool isLoop;
	int startTime;
	SpriteOrientation animDirection;

	AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true, SpriteOrientation animDirection = horizontal){
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->frameSpeedRate = frameSpeedRate;
		this->isLoop = isLoop;
		this->startTime = SDL_GetTicks();
		this->animDirection = animDirection;
	}
};
