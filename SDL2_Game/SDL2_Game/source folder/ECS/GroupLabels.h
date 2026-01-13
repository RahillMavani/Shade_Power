#pragma once
#include <cstddef> // Required for std::size_t definition

// Use an enum (a list of constant names) to define groups
enum groupLables : std::size_t 
{
	groupMap,       // Group 0: Entities belonging to the background map.
	groupPlayers,   // Group 1: The main player entity.
	groupEnemies,   // Group 2: All enemy entities
	groupColliders  // Group 3: All entities in collision detection (Walls, Player, Enemies).

};