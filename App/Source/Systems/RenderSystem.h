#pragma once
#include "ECS/System.h"
#include "ECS/Coordinator.h"
#include "Components.h"
#include <raylib.h>

extern Coordinator coordinator;

class RenderSystem : public System {
public:
    void Draw() {
        for (auto const& entity : entities) {
            auto& position = coordinator.GetComponent<Position>(entity);
            auto& sprite = coordinator.GetComponent<Sprite>(entity);
            DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), sprite.width, sprite.height, sprite.color);
        }
    }
};