#pragma once
#include <SDL3/SDL.h>
#include <memory>

// Set 0 to disable FPS lock
const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Registry;

class Game 
{
private:
	bool IsRunning;
	int MillisecondsPreviousFrame = 0;
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;

	std::unique_ptr<Registry> Registry_;

public:
	Game();
	~Game();

	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int WindowWidth = 800;
	int WindowHeight = 600;
};
