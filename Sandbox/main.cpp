#include "../Engine/include/Coordinator.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "../Engine/include/Components.h"
#include <raylib.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <rlImGui.h>
#include <memory>

Coordinator coordinator;

int main() {

    SetTraceLogLevel(LOG_WARNING); // Disables built in Raylib logs

    const int screenWidth = 1280;
    const int screenHeight = 720;
    spdlog::info("Initializing window...");
    InitWindow(screenWidth, screenHeight, "tecs");
    if (WindowShouldClose()) {
        spdlog::error("Window failed to initialize.");
    }
    spdlog::info("Window initialized.");

    SetTargetFPS(120);

    rlImGuiSetup(true);

    coordinator.RegisterComponent<Position>();
    coordinator.RegisterComponent<Velocity>();
    coordinator.RegisterComponent<Sprite>();

    auto physicsSystem = coordinator.RegisterSystem<PhysicsSystem>();
    Signature physicsSignature;
    physicsSignature.set(coordinator.GetComponentType<Position>());
    physicsSignature.set(coordinator.GetComponentType<Velocity>());
    coordinator.SetSystemSignature<PhysicsSystem>(physicsSignature);

    auto renderSystem = coordinator.RegisterSystem<RenderSystem>();
    Signature renderSignature;
    renderSignature.set(coordinator.GetComponentType<Position>());
    renderSignature.set(coordinator.GetComponentType<Sprite>());
    coordinator.SetSystemSignature<RenderSystem>(renderSignature);

    Entity player = coordinator.CreateEntity();
    coordinator.AddComponent<Position>(player, Position{ 0.0f, 0.0f });
    coordinator.AddComponent<Velocity>(player, Velocity{ 10.0f, 15.0f });
    coordinator.AddComponent<Sprite>(player, Sprite{ 20, 20, RED });

    int frameCount = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        physicsSystem->Update(dt);

        auto& playerPos = coordinator.GetComponent<Position>(player);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        renderSystem->Draw();

        rlImGuiBegin();
        ImGui::Begin("tecs");
        ImGui::Text("Current FPS: %d", GetFPS());
        ImGui::Text("Frame %d", frameCount);
        ImGui::Text("Position X: %.2f", playerPos.x);
        ImGui::SameLine();
        ImGui::Text(" Y: %.2f", playerPos.y);
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