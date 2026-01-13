#pragma once
// --- MODIFICATION LOG (Animation System) ---
// 1. New Class: This  was created to add all animation logic.
// 2. Separation of Concerns: It replaced the animation and sprite flip logic that was placed in the KeyboardEvent component in the  tutorial structure.

#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "GroupLabels.h" // Needed to reference groupPlayers

class AnimationSystem
{
private:
	Manager& manager; // Reference to the central entity manager.

public:
	// Constructor.
	AnimationSystem(Manager& mManager) : manager(mManager) {}

	// Runs every frame to check the state of all animated entities.
	void update(float dt)
	{

		// Get the list of entities that need animation processing (e.g., the player).
		auto& animatingEntities = manager.getGroup(groupPlayers);

		// Get the current state of the keyboard
		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		static Uint32 shootEndTime = 0;

		for (auto& entity : animatingEntities)
		{
			// Ensure the entity has the required data (Transform for movement, Sprite for visuals).
			if (entity->hasComponent<TransformComponent>() && entity->hasComponent<SpriteComponent>())
			{
				// Get the data components
				TransformComponent& transform = entity->getComponent<TransformComponent>();
				SpriteComponent& sprite = entity->getComponent<SpriteComponent>();

				// Animation State Logic 
				// Check if the entity is moving (velocity is non-zero).
				if (transform.velocity.x != 0 || transform.velocity.y != 0)
				{
					sprite.Play("Run"); // Play the running animation.
				}
				else
				{
					sprite.Play("Idle"); // Play the idle animation.
				}

				// Sprite Flipping Logic 
				// Check which direction the entity is moving to flip the sprite accordingly.
				if (transform.velocity.x < 0) // Moving Left
				{
					sprite.spriteFlip = SDL_FLIP_HORIZONTAL; // Flip the image horizontally.
				}
				else if (transform.velocity.x > 0) // Moving Right
				{
					sprite.spriteFlip = SDL_FLIP_NONE; // Keep the image its original orientation.
				}
			}
		}
	}
};