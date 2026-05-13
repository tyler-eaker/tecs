#include <ecs/Coordinator.h>
#include "systems/PhysicsSystem.h"
#include "systems/RenderSystem.h"
#include "Constants.h"
#include "Components.h"
#include <raylib.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <rlImGui.h>
#include <memory>
#include <vector>

Coordinator coordinator;

int main() {

    SetTraceLogLevel(LOG_WARNING); // Disables built in Raylib logs

    spdlog::info("Initializing window...");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "tecs");
    if (WindowShouldClose()) {
        spdlog::error("Window failed to initialize.");
    }
    spdlog::info("Window initialized.");

    SetTargetFPS(FPS_LIMIT);

    rlImGuiSetup(true);

    coordinator.RegisterComponent<Position>();
    coordinator.RegisterComponent<Velocity>();
    coordinator.RegisterComponent<Sprite>();

    auto physicsSystem = coordinator.RegisterSystem<PhysicsSystem>();
    Signature physicsSignature;
    physicsSignature.set(coordinator.GetComponentType<Position>());
    physicsSignature.set(coordinator.GetComponentType<Velocity>());
    coordinator.SetSystemSignature<PhysicsSystem>(physicsSignature);
    bool physicsEnabled = true;

    auto renderSystem = coordinator.RegisterSystem<RenderSystem>();
    Signature renderSignature;
    renderSignature.set(coordinator.GetComponentType<Position>());
    renderSignature.set(coordinator.GetComponentType<Sprite>());
    coordinator.SetSystemSignature<RenderSystem>(renderSignature);

    int targetHundreds = 1;
    std::vector<Entity> activeSwarm;

    int frameCount = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        int targetEntities = targetHundreds * 100;

        while (activeSwarm.size() < static_cast<uint32_t>(targetEntities)) {
            Entity entity = coordinator.CreateEntity();
            coordinator.AddComponent<Position>(entity, Position{ static_cast<float>(GetRandomValue(0, SCREEN_WIDTH)), static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT)) });
            coordinator.AddComponent<Velocity>(entity, Velocity{ static_cast<float>(GetRandomValue(-500.0f, 500.0f)), static_cast<float>(GetRandomValue(-50.0f, 50.0f)) });
            coordinator.AddComponent<Sprite>(entity, Sprite{ static_cast<uint32_t>(GetRandomValue(1, 5)), static_cast<uint32_t>(GetRandomValue(1, 5)), COLORS[GetRandomValue(0, MAX_COLORS_COUNT - 1)] });
            activeSwarm.push_back(entity);
        }

        while (activeSwarm.size() > static_cast<uint32_t>(targetEntities)) {
            Entity entityToDestroy = activeSwarm.back();
            coordinator.DestroyEntity(entityToDestroy);
            activeSwarm.pop_back();
        }

        if (physicsEnabled) {
            physicsSystem->Update(dt, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        BeginDrawing();
        ClearBackground(CLEAR_BACKGROUND_COLOR);

        renderSystem->Draw();

        rlImGuiBegin();
        ImGui::Begin("tecs");
        ImGui::Text("Current FPS: %d", GetFPS());
        ImGui::Text("Frame %d", frameCount);
        ImGui::Text("Entity count: %zu", activeSwarm.size());
        ImGui::SliderInt("Swarm Size", &targetHundreds, 1, MAX_ENTITIES / 100);
        ImGui::Checkbox("Enable physics", &physicsEnabled);
        ImGui::End();
        rlImGuiEnd();

        EndDrawing();

        frameCount++;
    }

    rlImGuiShutdown();
    CloseWindow();
    spdlog::info("Engine shut down successfully.");

    return 0;
}