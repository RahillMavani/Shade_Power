#pragma once
// --- MODIFICATION LOG -
// 1. Decoupled Time: The 'update()' signature was changed to accept 'float dt'.
// 2. Code Quality: Added member initializer lists to all constructors to fix C26495 warnings.

#include "ECS.h"
#include "../Header files/Vector2D.h"
#include "Header files/GameLoop.h"

class TransformComponent : public Component
{
public:
	// Data Fields
	Vector2D position; // The current X/Y coordinate of the entity
	Vector2D velocity; // The current direction of movement (-1 to 1)

	int height = 32; // Default height of the sprite sheet frame
	int width = 32;  // Default width of the sprite sheet frame
	int scale = 1;   // Scaling factor (e.g., 2 means the object is 2x the size)

	float speed = 150.0f; // Movement speed factor (pixels per second)

	//Constructor to initialize all members (Fixes C26495 warnings)
	TransformComponent() : velocity(0, 0), speed(150.0f) {}

	// Parameterized Constructor
	TransformComponent(float x, float y, int h, int w, int s)
		: position(x, y), height(h), width(w), scale(s), velocity(0, 0), speed(150.0f)
	{
	}

	// Overloaded Parameterized Constructor (allows setting speed)
	TransformComponent(float x, float y, int h, int w, int s, float sp)
		: position(x, y), height(h), width(w), scale(s), velocity(0, 0), speed(sp)
	{
	}

	void init() override
	{
		// Runs once when created
	}

	void update(float dt) override // *** MODIFIED: Accepts dt instead of relying on global time ***
	{
		// --- Core Movement Logic ---
		// New Position = Old Position + (Velocity * Speed * Time Elapsed (dt))
		position.x += velocity.x * speed * dt;
		position.y += velocity.y * speed * dt;
	}
};