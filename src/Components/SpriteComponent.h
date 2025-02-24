#pragma once

struct SpriteComponent 
{
	int width;
	int height;

	SpriteComponent(int width = 32, int height = 32)
	{
		this->width = width;
		this->height = height;
	}
};
