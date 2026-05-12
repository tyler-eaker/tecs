#pragma once
#include "../Engine/include/System.h"
#include "../Engine/include/Coordinator.h"
#include "../Engine/include/Components.h"

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