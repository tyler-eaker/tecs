#include "../Engine/include/Coordinator.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "../Engine/include/Components.h"
#include <raylib.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <rlImGui.h>
#include <memory>

#define MAX_COLORS_COUNT 21

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

    Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };

    const char* colorNames[MAX_COLORS_COUNT] = {
        "DARKGRAY", "MAROON", "ORANGE", "DARKGREEN", "DARKBLUE", "DARKPURPLE",
        "DARKBROWN", "GRAY", "RED", "GOLD", "LIME", "BLUE", "VIOLET", "BROWN",
        "LIGHTGRAY", "PINK", "YELLOW", "GREEN", "SKYBLUE", "PURPLE", "BEIGE" };

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

    uint32_t entitiesToSpawn = 5000;
    uint32_t currentEntities = 0;
    for (uint32_t testEntities = 0; testEntities < entitiesToSpawn; ++testEntities) {
        Entity entity = coordinator.CreateEntity();
        coordinator.AddComponent<Position>(entity, Position{ static_cast<float>(GetRandomValue(0, screenWidth)), static_cast<float>(GetRandomValue(0, screenHeight)) });
        coordinator.AddComponent<Velocity>(entity, Velocity{ static_cast<float>(GetRandomValue(-500.0f, 500.0f)), static_cast<float>(GetRandomValue(-50.0f, 50.0f)) });
        coordinator.AddComponent<Sprite>(entity, Sprite{ 20, 20, colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)]});
        ++currentEntities;
    }

    int frameCount = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        physicsSystem->Update(dt, screenWidth, screenHeight);

        //auto& playerPos = coordinator.GetComponent<Position>(player);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        renderSystem->Draw();

        rlImGuiBegin();
        ImGui::Begin("tecs");
        ImGui::Text("Current FPS: %d", GetFPS());
        ImGui::Text("Frame %d", frameCount);
        ImGui::Text("Entity count: %d", currentEntities);
        //ImGui::Text("Position X: %.2f", playerPos.x);
        //ImGui::SameLine();
        //ImGui::Text(" Y: %.2f", playerPos.y);
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