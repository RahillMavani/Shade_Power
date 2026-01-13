#include "Header files/Collision.h"
#include "ECS/ColliderComponent.h"
#include <iostream> // Needed for the console output

// -Collision Logic Implementation (Checks if two boxes hit) 

// AABB Overload for SDL_Rect (The core math function)
// Checks if two simple rectangles (rA and rB) overlap.
bool Collision::AABB(const SDL_Rect& rA, const SDL_Rect& rB)
{
	// If they are NOT separate, they must be colliding.
	if (
		// Check 1: Is the right side of box A past the left side of box B?
		rA.x + rA.w >= rB.x &&

		// Check 2: Is the right side of box B past the left side of box A?
		rB.x + rB.w >= rA.x &&

		// Check 3: Is the bottom side of box A past the top side of box B?
		rA.y + rA.h >= rB.y &&

		// Check 4: Is the bottom side of box B past the top side of box A?
		rB.y + rB.h >= rA.y
		)
	{
		// If all checks are true, they are overlapping. 
		return true;
	}
	return false;
}

// AABB Overload for ColliderComponent (The ECS helper function)
// lets the game check collision using the full component data.
bool Collision::AABB(const ColliderComponent& cA, const ColliderComponent& cB)
{
	// Call the above using the 'collider' box inside each component.
	if (AABB(cA.collider, cB.collider))
	{
		// Print a message to the console when a collision happens (useful for debugging).
		std::cout << cA.tag << " hit: " << cB.tag << std::endl;
		return true;
	}
	else return false;
}