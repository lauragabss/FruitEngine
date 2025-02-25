#include "AssetManager.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../Logger/Logger.h"

AssetManager::AssetManager()
{
	Logger::Log("Asset manager constructor called!");
}

AssetManager::~AssetManager()
{
	ClearAssets();
	Logger::Log("Asset manager destructor called!");
}

void AssetManager::ClearAssets()
{
	for (auto texture : Textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	Textures.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);

	// Set scale mode to keep resolution of pixel art
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	Textures.emplace(assetId, texture);

	Logger::Log("New texture added to the assets manager");
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId)
{
	return Textures[assetId];
}
