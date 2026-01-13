#pragma once
#include "Components.h"
#include "../Header files/Vector2D.h"

class ProjectileComponent : public Component {
public:
    //how fast, long it can travel and stuff
    int range = 0;
    int speed = 1;
    int distance = 0;
    Vector2D velocity;

    ProjectileComponent(int rng, int sp, Vector2D vel)
        : range(rng), speed(sp), velocity(vel), distance(0) {
    }

    void update(float dt) override {
        // Logic for despawning based on range
        distance += speed;

        if (distance > range) {
            entity->destroy(); // Removes the bullet once it travels too far
        }
    }
};