#include "../Engine/include/Coordinator.h"
#include "PhysicsSystem.h"
#include "../Engine/include/Components.h"
#include <raylib.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <rlImGui.h>
#include <memory>

Coordinator coordinator;

int main() {

    SetTraceLogLevel(LOG_WARNING); // Disables built in Raylib logs
    spdlog::info("Starting tecs...");

    const int screenWidth = 1280;
    const int screenHeight = 720;
    spdlog::info("Initializing window...");
    InitWindow(screenWidth, screenHeight, "tecs");
    if (WindowShouldClose()) {
        spdlog::error("Window failed to initialize.");
    }
    spdlog::info("Window initialized.");

    SetTargetFPS(60);

    rlImGuiSetup(true);

    coordinator.RegisterComponent<Position>();
    coordinator.RegisterComponent<Velocity>();
    auto physicsSystem = coordinator.RegisterSystem<PhysicsSystem>();


    Signature physicsSignature;
    physicsSignature.set(coordinator.GetComponentType<Position>());
    physicsSignature.set(coordinator.GetComponentType<Velocity>());
    coordinator.SetSystemSignature<PhysicsSystem>(physicsSignature);

    Entity player = coordinator.CreateEntity();

    coordinator.AddComponent<Position>(player, Position{ 0.0f, 0.0f });

    coordinator.AddComponent<Velocity>(player, Velocity{ 10.0f, 15.0f });

    int frameCount = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        physicsSystem->Update(dt);

        auto& playerPos = coordinator.GetComponent<Position>(player);
        spdlog::info("Frame {}: Position X: {:.2f}, Y: {:.2f}", frameCount, playerPos.x, playerPos.y);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        ImGui::Begin("tecs");
        ImGui::Text("Engine Core Systems Initialized.");
        ImGui::Text("Current FPS: %d", GetFPS());
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