#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Systems/MovementSystem.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

Game::Game()
{
	Logger::Log("Constructor Game called");
	Registry_ = std::make_unique<Registry>();

	IsRunning = false;
}

Game::~Game()
{
	// TODO
	Logger::Log("Game destructor called");
}

void Game::Initialize()
{
	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_CAMERA | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_SENSOR | SDL_INIT_VIDEO))
	{
		Logger::Err("Error initializing SDL.");
		return;
	}

	Window = SDL_CreateWindow(
		"Fruit Engine",
		WindowWidth,
		WindowHeight,
		SDL_WINDOW_BORDERLESS
		);


	if (!Window)
	{
		Logger::Err("Error creating SDL window.");
		return;
	}

	SDL_SetWindowFullscreen(Window, true);

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	Renderer = SDL_CreateRenderer(Window, NULL);
	if (!Renderer)
	{
		Logger::Err("Error creating SDL renderer.");
		return;
	}

	IsRunning = true;
}

void Game::Run()
{
	while (IsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Setup()
{
	// Add the systems that need to be processed in the game
	Registry_->AddSystem<MovementSystem>();

	// Create some entities
	Entity fruit = Registry_->CreateEntity();

	fruit.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	fruit.AddComponent<RigidBodyComponent>(glm::vec2(5.0, 1.0));
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch(sdlEvent.type)
		{
		case SDL_EVENT_QUIT:
			IsRunning = false;
			break;

		case SDL_EVENT_KEY_DOWN:
			if(sdlEvent.key.key == SDLK_ESCAPE)
			{
				IsRunning = false;
			}
			break;
		}
	}
	
}


void Game::Update()
{
	// Wait frame time to coninue according to aimed FPS
	if (FPS != 0)
	{
		int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - MillisecondsPreviousFrame);
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
		{
			SDL_Delay(timeToWait);
		}
	}

	// Time passed between frames in seconds
	double deltatime = (SDL_GetTicks() - MillisecondsPreviousFrame) / 1000.0;

	// Store current frame time
	MillisecondsPreviousFrame = SDL_GetTicks();

	// Ask all Systems to update
	Registry_->GetSystem<MovementSystem>().Update(deltatime);

	// Update the Registry (to process the entities waiting to be created/deleted) - this has to be tha last task of the frame
	Registry_->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 200, 100, 255, 255);
	SDL_RenderClear(Renderer);

	SDL_RenderPresent(Renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}
