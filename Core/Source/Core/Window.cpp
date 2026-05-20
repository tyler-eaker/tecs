#include "Window.h"

namespace Core 
{
    Window::Window(const WindowSpecification& specification)
        : m_Specification(specification) 
    {
    }

    Window::~Window() 
    {
        Destroy();
    }

    void Window::Create() 
    {
        SetTraceLogLevel(LOG_WARNING);

        spdlog::info("Initializing window...");
        InitWindow(m_Specification.width, m_Specification.height, m_Specification.title);
        if (WindowShouldClose()) {
            spdlog::error("Window failed to initialize.");
        }
        spdlog::info("Window initialized.");

        SetTargetFPS(m_Specification.fpsLimit);

        if (m_Specification.isResizable == true)
            SetWindowState(FLAG_WINDOW_RESIZABLE);
    }

    void Window::Destroy() 
    {
        if (IsWindowReady()) {
            CloseWindow();
        }
    }

    void Window::BeginFrame() 
    {
        BeginDrawing();
    }

    void Window::EndFrame() 
    {
        EndDrawing();
    }

    bool Window::ShouldClose() const 
    {
        return WindowShouldClose();
    }
}