#pragma once
// --- MODIFICATION LOG ---
// 1. Separation of Concerns: Removed all animation and sprite flip logic. 
//    This component now only updates velocity (state). Visuals are handled by AnimationSystem.
// 2. Code Quality: Added member initializer list to fix C26495 warnings.

#include "GameLoop.h"
#include "../ECS/ECS.h"
#include "../ECS/Components.h"

class KeyboardEvent : public Component
{
public:
	TransformComponent* transform; // Pointer to the entity's position/velocity data.
	SpriteComponent* sprite;       // Pointer to the entity's visual data.

	//Constructor to initialize pointers to nullptr (Fixes C26495 warnings)
	KeyboardEvent() : transform(nullptr), sprite(nullptr) {}

	void init() override
	{
		// Get references to the Transform and Sprite components during creation.
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update(float dt) override
	{
		// Get the current state of all keys on the keyboard.
		const Uint8* state = SDL_GetKeyboardState(NULL);

		// Reset velocity before checking keys (ensures stopping when keys are released).
		transform->velocity.x = 0;
		transform->velocity.y = 0;

		//Vertical Movement Input
		if (state[SDL_SCANCODE_W])
		{
			transform->velocity.y = -1; // Set normalized velocity up.
		}
		if (state[SDL_SCANCODE_S])
		{
			transform->velocity.y = 1; // Set normalized velocity down.
		}

		//Horizontal Movement Input
		if (state[SDL_SCANCODE_A])
		{
			transform->velocity.x = -1; // Set normalized velocity left.
		}
		if (state[SDL_SCANCODE_D])
		{
			transform->velocity.x = 1; // Set normalized velocity right.
		}
	}
};