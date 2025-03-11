#pragma once
#include <SDL3/SDL.h>
#include <memory>

// Set 0 to disable FPS lock
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Registry;
class AssetManager;
class EventBus;

class Game 
{
private:
	bool IsRunning;
	bool IsDebug;
	int MillisecondsPreviousFrame = 0;
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	SDL_Rect Camera;

	std::unique_ptr<Registry> Registry_;
	std::unique_ptr<AssetManager> AssetManager_;
	std::unique_ptr<EventBus> EventBus_;

public:
	Game();
	~Game();

	void Initialize();
	void Run();
	void LoadLevel(int level);
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	static int WindowWidth;
	static int WindowHeight;
	static int MapWidth;
	static int MapHeight;
};
