#pragma once

#include <raylib.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Core {

    struct WindowSpecification {
        const char* title = "Application";
        uint32_t width = 1280;
        uint32_t height = 720;
        uint32_t fpsLimit = 120;
        bool isResizable = true;
    };

    class Window
    {
    public:
        Window(const WindowSpecification& specification = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();

        void BeginFrame();
        void EndFrame();
        bool ShouldClose() const;

    private:
        WindowSpecification m_Specification;
    };
}