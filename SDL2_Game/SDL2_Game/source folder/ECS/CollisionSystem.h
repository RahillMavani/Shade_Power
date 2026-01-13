#pragma once
// --- MODIFICATION LOG (Collision System) ---
// 1. New Class: created to house all collision logic.
// 2. Decoupling: Replaced the old structure's manual iteration over static lists.
//    Logic now uses Manager::getGroup(groupColliders) to find objects.
// 3. Dependency Injection: Accepts 'oldPosition' to enable correct collision resolution (snapping back).

#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../Header files/Collision.h" // Provides the AABB math function
#include "../Header files/Vector2D.h"  // Provides the Vector2D struct for position tracking
#include "GroupLabels.h"             // Provides the groupColliders constant
#include <iostream>

class CollisionSystem
{
private:
    Manager& manager; // Reference to the central entity manager.

public:
    // Constructor.
    CollisionSystem(Manager& mManager) : manager(mManager) {}

    // Runs every frame to check for collisions against environmental objects (like walls).
    // playerEntity: The entity being moved (e.g., the player).
    // oldPosition: The position *before* the current frame's movement (used to resolve hits).
    void update(Entity* playerEntity, const Vector2D& oldPosition)
    {
        // Get the player's data components.
        TransformComponent& playerTransform = playerEntity->getComponent<TransformComponent>();
        ColliderComponent& playerCollider = playerEntity->getComponent<ColliderComponent>();

        // Query the Manager for the list of ALL entities that are collidable (Walls, enemies, etc.).
        auto& collidableEntities = manager.getGroup(groupColliders);

        // Loop through every potential collision object.
        for (auto& wall : collidableEntities)
        {
            if (wall == playerEntity) continue;

            // Safety check: skip if the target doesn't have a collider
            if (!wall->hasComponent<ColliderComponent>()) continue;

            std::string wallTag = wall->getComponent<ColliderComponent>().tag;
            std::string selfTag = playerCollider.tag;

            // Filter out player/projectile internal hits
            if ((selfTag == "projectile" && wallTag == "player") ||
                (selfTag == "player" && wallTag == "projectile"))
            {
                continue;
            }

            // Check for bullet hitting an enemy or wall
            if (Collision::AABB(playerCollider, wall->getComponent<ColliderComponent>()))
            {
                if (selfTag == "projectile") {
                    std::cout << "Bullet hit something!" << std::endl;
                    playerEntity->destroy(); // Destroy the bullet on impact
                    if (wallTag == "enemy") wall->destroy(); // Kill the demon!
                    break;
                }

                playerTransform.position = oldPosition;
                break;
            }
        }
    }
};