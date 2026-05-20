#pragma once

#include <ECS/System.h>
#include <ECS/Coordinator.h>
#include <Core/AssetManager.h>
#include "../Components.h"
#include <raylib.h>

extern Coordinator coordinator;

class RenderSystem : public System {
public:
    void Draw() {
        for (auto const& entity : entities) {
            auto& position = coordinator.GetComponent<Position>(entity);
            auto& sprite = coordinator.GetComponent<Sprite>(entity);

            if (!sprite.textureId.empty()) {
                Texture2D tex = Core::AssetManager::Get().GetTexture(sprite.textureId);

                Rectangle source = { 0.0f, 0.0f, static_cast<float>(tex.width), static_cast<float>(tex.height) };
                Rectangle dest = { position.x, position.y, static_cast<float>(sprite.width), static_cast<float>(sprite.height) };
                Vector2 origin = { 0.0f, 0.0f };

                DrawTexturePro(tex, source, dest, origin, 0.0f, sprite.tint);
            }
            else {
                DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), sprite.width, sprite.height, sprite.tint);
            }
        }
    }
};