#include "Application.h"

namespace Core
{
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        s_Application = this;

        if (m_Specification.windowSpec.title == nullptr || m_Specification.windowSpec.title[0] == '\0')
            m_Specification.windowSpec.title = m_Specification.name;

        m_Window = std::make_shared<Window>(m_Specification.windowSpec);
        m_Window->Create();

        rlImGuiSetup(true);
    }

    Application::~Application()
    {
        for (auto& layer : m_LayerStack) {
            layer->OnDetach();
        }
        m_LayerStack.clear();

        // FLUSH GPU MEMORY BEFORE DESTROYING THE WINDOW
        AssetManager::Get().Clear();

        rlImGuiShutdown();
        m_Window->Destroy();
        s_Application = nullptr;
    }

    void Application::Run()
    {
        m_Running = true;
        float lastTime = GetTime();

        while (m_Running) {

            PollRaylibEvents();

            if (!m_Running) break;

            float currentTime = GetTime();
            float timestep = std::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            for (auto& layer : m_LayerStack) {
                layer->OnUpdate(timestep);
            }

            m_Window->BeginFrame();
            ClearBackground(BLACK);
            for (auto& layer : m_LayerStack) {
                layer->OnRender();
            }

            rlImGuiBegin();
            for (auto& layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            rlImGuiEnd();

            m_Window->EndFrame();
        }
    }

    void Application::Stop()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return OnWindowClose(event); });

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    void Application::PollRaylibEvents()
    {
        if (WindowShouldClose()) {
            WindowCloseEvent e;
            OnEvent(e);
        }

        if (IsWindowResized()) {
            WindowResizeEvent e(GetScreenWidth(), GetScreenHeight());
            OnEvent(e);
        }

        int key = GetKeyPressed();
        while (key != 0) {
            KeyPressedEvent e(key);
            OnEvent(e);
            key = GetKeyPressed();
        }

        Vector2 mouseDelta = GetMouseDelta();
        if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f) {
            MouseMovedEvent e(static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()));
            OnEvent(e);
        }
    }

    Application& Application::Get()
    {
        return *s_Application;
    }

    float Application::GetTime()
    {
        return static_cast<float>(::GetTime());
    }
}