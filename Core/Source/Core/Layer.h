#pragma once
#include "Events/Event.h"
#include <string>

namespace Core {
    class Layer {
    public:
        Layer(const std::string& name = "Layer") : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float timestep) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}