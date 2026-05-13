#pragma once
#include "../Engine/include/System.h"
#include "../Engine/include/Coordinator.h"
#include "../Engine/include/Components.h"

extern Coordinator coordinator;

class PhysicsSystem : public System {
public:
    void Update(float dt, int screen_w, int screen_h) {
        for (auto const& entity : entities) {
            auto& position = coordinator.GetComponent<Position>(entity);
            auto& velocity = coordinator.GetComponent<Velocity>(entity);
            position.x += (velocity.x * dt);
            position.y += (velocity.y * dt);
            if (position.x < 0 || position.x > screen_w) {
                velocity.x *= -1.0f;
            }
            if (position.y < 0 || position.y > screen_h) {
                velocity.y *= -1.0f;
            }
        }
    }
};