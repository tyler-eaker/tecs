#include "Window.h"

namespace Core {

	Window::Window(const WindowSpecification& specification)
		: m_Specification(specification) {
	}

	Window::~Window() {
		Destroy();
	}

	void Window::Create() {
		spdlog::info("Initializing window...");
		InitWindow(m_Specification.width, m_Specification.height, m_Specification.title);
		if (WindowShouldClose()) {
			spdlog::error("Window failed to initialize.");
		}
		spdlog::info("Window initialized.");

		SetTargetFPS(m_Specification.fpsLimit);
	}

	void Window::Destroy() {
		if (IsWindowReady()) {
			CloseWindow();
			spdlog::info("Window closed safely.");
		}
	}

	void Window::BeginFrame() {
		BeginDrawing();
	}

	void Window::EndFrame() {
		EndDrawing();
	}

	bool Window::ShouldClose() const
	{
		return WindowShouldClose();
	}
}