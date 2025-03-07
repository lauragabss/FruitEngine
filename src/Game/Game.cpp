#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/RenderSystem.h"
#include "../AssetManager/AssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

Game::Game()
{
	Logger::Log("Constructor Game called");
	Registry_ = std::make_unique<Registry>();
	AssetManager_ = std::make_unique<AssetManager>();
	EventBus_ = std::make_unique<EventBus>();

	IsRunning = false;
	IsDebug = false;
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

void Game::LoadLevel(int level)
{
	// Add the systems that need to be processed in the game
	Registry_->AddSystem<MovementSystem>();
	Registry_->AddSystem<RenderSystem>();
	Registry_->AddSystem<AnimationSystem>();
	Registry_->AddSystem<CollisionSystem>();
	Registry_->AddSystem<RenderColliderSystem>();
	Registry_->AddSystem<DamageSystem>();
	Registry_->AddSystem<KeyboardControlSystem>();

	// Adding assets to the asset manager
	AssetManager_->AddTexture(Renderer, "fruit-image", "./assets/images/FrutinhaOriginalSize.png");
	AssetManager_->AddTexture(Renderer, "tank-image", "./assets/images/tank-panther-right.png");
	AssetManager_->AddTexture(Renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	AssetManager_->AddTexture(Renderer, "radar-image", "./assets/images/radar.png");
	AssetManager_->AddTexture(Renderer, "potion-image", "./assets/images/Potion.png");

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// MAP BACKGROUND
	/////////////////////////////////////////////////////////////////////////////////////////////////
	AssetManager_->AddTexture(Renderer, "jungle-tilemap", "./assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;

	std::ifstream mapFile("./assets/tilemaps/jungle.map");
	// mapFile.open("./assets/tilemaps/jungle.map");
	if (!mapFile)
	{
		Logger::Err("Couldn't open map file");
	}
	else
	{
		std::string line, value;

		int mapNumCols = 1;

		for (int j = 0; j < mapNumCols; j++)
		{

			if (std::getline(mapFile, line))
			{
				std::string value;
				int i = 0;
				for (char c : line)
				{
					if (c == ',' || c == ';')
					{

						Entity tile = Registry_->CreateEntity();
						// Position / Scale / Rotation
						tile.AddComponent<TransformComponent>(glm::vec2(i * (tileSize * tileScale), j * (tileSize * tileScale)), glm::vec2(tileScale, tileScale));

						// Texture / SizeX / Size Y / Source X / Source Y
						int sourceX = ((value[1] - '0') * tileSize);
						int sourceY = ((value[0] - '0') * tileSize);
						tile.AddComponent<SpriteComponent>("jungle-tilemap", tileSize, tileSize, 0, sourceX, sourceY);

						// Add for next position
						value = "";
						if (c == ';')
						{
							mapNumCols++;
						}
						i++;
					}
					else
					{
						value.push_back(c);
					}
				}
			}
		}
		mapFile.close();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// Create some entities
	Entity fruit = Registry_->CreateEntity();
	fruit.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(3.0, 3.0), 0.0);
	fruit.AddComponent<RigidBodyComponent>(glm::vec2(5.0, 3.0));
	fruit.AddComponent<SpriteComponent>("fruit-image", 32, 32, 2);

	Entity other = Registry_->CreateEntity();
	other.AddComponent<TransformComponent>(glm::vec2(300.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	other.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 0.0));
	other.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	other.AddComponent<AnimationComponent>(2, 10, true);
	other.AddComponent<BoxColliderComponent>(32,32);
	other.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -20), glm::vec2(20, 0), glm::vec2(0, 20), glm::vec2(-20,0));


	Entity radar = Registry_->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(WindowWidth - 74, 10.0), glm::vec2(2.0, 2.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 3);
	radar.AddComponent<AnimationComponent>(8, 10, true, horizontal);

	Entity potion = Registry_->CreateEntity();
	potion.AddComponent<TransformComponent>(glm::vec2(500.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	potion.AddComponent<RigidBodyComponent>(glm::vec2(-5.0, 0.0));
	potion.AddComponent<SpriteComponent>("potion-image", 16, 17, 1);
	potion.AddComponent<AnimationComponent>(4, 5, true, vertical);
	potion.AddComponent<BoxColliderComponent>(16, 17);
}

void Game::Setup()
{
	LoadLevel(1);
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
			EventBus_->EmitEvent<KeyPressedEvent>(sdlEvent.key.key);
			if(sdlEvent.key.key == SDLK_ESCAPE)
			{
				IsRunning = false;
			}
			if(sdlEvent.key.key == SDLK_D)
			{
				IsDebug = !IsDebug;
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

	// Reset event handlers for the frame
	EventBus_->Reset();

	// Subscription of events for all systems
	Registry_->GetSystem<DamageSystem>().SubscribeToEvents(EventBus_);
	Registry_->GetSystem<KeyboardControlSystem>().SubscribeToEvents(EventBus_);

	// Ask all Systems to update
	Registry_->GetSystem<MovementSystem>().Update(deltatime);
	Registry_->GetSystem<AnimationSystem>().Update();
	Registry_->GetSystem<CollisionSystem>().Update(EventBus_);
	Registry_->GetSystem<DamageSystem>().Update();
	Registry_->GetSystem<KeyboardControlSystem>().Update();

	// Update the Registry (to process the entities waiting to be created/deleted) - this has to be tha last task of the frame
	Registry_->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 200, 100, 255, 255);
	SDL_RenderClear(Renderer);

	// Invoke all the systems that need to render
	Registry_->GetSystem<RenderSystem>().Update(Renderer, *AssetManager_);

	if(IsDebug)
	{
		Registry_->GetSystem<RenderColliderSystem>().Update(Renderer);
	}

	SDL_RenderPresent(Renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}
