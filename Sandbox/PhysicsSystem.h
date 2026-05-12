#pragma once
#include "../Engine/include/System.h"
#include "../Engine/include/Coordinator.h"
#include "../Engine/include/Components.h"

extern Coordinator coordinator;

class PhysicsSystem : public System {
public:
    void Update(float dt) {
        for (auto const& entity : entities) {
            auto& position = coordinator.GetComponent<Position>(entity);
            auto& velocity = coordinator.GetComponent<Velocity>(entity);
            position.x += (velocity.x * dt);
            position.y += (velocity.y * dt);
        }
    }
};