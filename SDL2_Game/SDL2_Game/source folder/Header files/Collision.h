#pragma once
#include <SDL.h>

class ColliderComponent; // Forward declaration

class Collision
{
public:
	// Static tool to check collision between two standard SDL rectangles (Axis-Aligned Bounding Box check).
	static bool AABB(const SDL_Rect& rA, const SDL_Rect& rB);

	// Static tool to check collision between two ColliderComponents (uses the AABB function internally).
	static bool AABB(const ColliderComponent& cA, const ColliderComponent& cB);

};