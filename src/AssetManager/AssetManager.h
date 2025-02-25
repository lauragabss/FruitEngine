#pragma once

#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>

class AssetManager
{
private:
	std::unordered_map<std::string, SDL_Texture*> Textures;
	// TODO: create map for fonts and audio

public:
	AssetManager();
	~AssetManager();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);
};
