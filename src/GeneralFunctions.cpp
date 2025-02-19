#include <SDL3/SDL.h>

// Drawing a rectangle example
void DrawRectangle(SDL_Renderer* renderer, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, int posX, int posY, int sizeX, int sizeY)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
	const SDL_FRect player = { posX, posY, sizeX, sizeY };
	SDL_RenderFillRect(renderer, &player);
}
