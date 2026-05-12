#include <raylib.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <rlImGui.h>

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
    SetTargetFPS(9999);

    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        ImGui::Begin("tecs");
        ImGui::Text("Engine Core Systems Initialized.");
        ImGui::Text("Current FPS: %d", GetFPS());
        ImGui::End();
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    spdlog::info("Engine shut down cleanly.");

    return 0;
}