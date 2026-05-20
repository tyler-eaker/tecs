#pragma once

#include "Window.h"
#include "Layer.h"
#include "AssetManager.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

#include <algorithm>
#include <string>
#include <memory>
#include <vector>

namespace Core 
{

    struct ApplicationSpecification 
    {
        const char* name = "Application";
        WindowSpecification windowSpec;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification = ApplicationSpecification());
        ~Application();

        void Run();
        void Stop();
        void OnEvent(Event& e);

        template<typename TLayer>
        void PushLayer() 
        {
            m_LayerStack.push_back(std::make_shared<TLayer>());
            m_LayerStack.back()->OnAttach();
        }

        std::shared_ptr<Window> GetWindow() const { return m_Window; }

        static Application& Get();
        static float GetTime();

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        void PollRaylibEvents();

        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_Window;
        bool m_Running = false;

        std::vector<std::shared_ptr<Layer>> m_LayerStack;
    };
}