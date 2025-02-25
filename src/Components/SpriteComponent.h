#pragma once
#include <string>
#include <SDL3/SDL.h>

struct SpriteComponent 
{
	std::string assetId;
	int width;
	int height;
	SDL_FRect sourceRec;

	SpriteComponent(std::string assetId = "", int width = 32, int height = 32, int srcRecX = 0, int srcRecY = 0)
	{
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->sourceRec = { static_cast<float>(srcRecX), static_cast<float>(srcRecY),
							static_cast<float>(width), static_cast<float>(height) };
	}
};
