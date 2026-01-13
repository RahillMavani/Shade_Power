#pragma once
// --- MODIFICATION LOG (Collider Component) ---
// 1. Decoupling: Removed logic that added 'this' to the static GameLoop::colliders list (the component no longer self-registers).
//    Collision checking is now handled externally by the dedicated CollisionSystem.
// 2. Code Quality: Added member initializer list to fix C26495 warnings (uninitialized members).

#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;          // The collision box dimensions and position (data).
	std::string tag;            // Identifier for the box ("wall", "player").

	TransformComponent* transform; // Pointer to the entity's position data.

	// Constructor to initialize all members safely (Fixes C26495 warnings).
	ColliderComponent(std::string t)
		: tag(t), collider({ 0, 0, 0, 0 }), transform(nullptr)
	{
	}

	void init() override
	{
		// Ensures there is a TransformComponent to position the collider (standard ECS dependency check).
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		// *** TUTORIAL CODE REMOVED HERE: GameLoop::colliders.push_back(this); *
	}

	void update(float dt) override
	{
		// Update the collider's position based on the TransformComponent data.
		// using static_cast<int> to explicitly convert float positions to integer pixels (Fixes C4244 warnings).
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);

		// Update size based on the TransformComponent's width, height, and scale.
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};