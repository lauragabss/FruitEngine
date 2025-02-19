#include "Game.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

Game::Game()
{
	// TODO
	printf("\n Constructor Game called \n");
	IsRunning = false;
}

Game::~Game()
{
	// TODO
	printf("\n Game destructor called \n");
}

void Game::Initialize()
{
	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_CAMERA | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_SENSOR | SDL_INIT_VIDEO))
	{
		std::cerr << "Error initializing SDL." << std::endl;
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
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	SDL_SetWindowFullscreen(Window, true);

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	Renderer = SDL_CreateRenderer(Window, NULL);
	if (!Renderer)
	{
		std::cerr << "Error creating SDL renderer." << std::endl;
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
	// TODO: Initialize game objects
	playerPosition = glm::vec2(10.0, 20.0);

	// Pixels per second
	playerVelocity = glm::vec2(20.0, 50.0);
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
	double deltaTime = (SDL_GetTicks() - MillisecondsPreviousFrame) / 1000.0;

	// Store current frame time
	MillisecondsPreviousFrame = SDL_GetTicks();

	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 200, 100, 255, 255);
	SDL_RenderClear(Renderer);

	SDL_Surface* surface = IMG_Load("./assets/images/Potion.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
	SDL_DestroySurface(surface);

	SDL_FRect rect1;
	rect1.x = playerPosition.x;
	rect1.y = playerPosition.y;
	rect1.w = texture->w;
	rect1.h = texture->h;

	SDL_RenderTexture(Renderer, texture, NULL, &rect1);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(Renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}
