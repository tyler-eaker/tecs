#pragma once

#include "Events/Event.h"

#include <string>

namespace Core 
{
    class Layer 
    {
    public:
        Layer(const std::string& name = "Layer") : m_DebugName(name) { spdlog::info("Creating layer: {}", m_DebugName); }
        virtual ~Layer() = default;

        virtual void OnAttach() {};                 // Initialization phase
        virtual void OnDetach() {};                 // Cleanup phase
        virtual void OnUpdate(float timestep) {};   // Runs every frame (Math & logic)
        virtual void OnRender() {};                 // Runs every frame (Graphics)
        virtual void OnImGuiRender() {};            // Runs every frame (ImGui elements)
        virtual void OnEvent(Event& event) {};      // Runs every time event is triggered (Mouse, keys, resize events etc.)

        const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}