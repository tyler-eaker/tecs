#pragma once

#include "Event.h"

namespace Core 
{
    class MouseMovedEvent : public Event 
    {
    public:
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}
        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        static EventType GetStaticType() { return EventType::MouseMoved; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "MouseMoved"; }
        virtual int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
    private:
        float m_MouseX, m_MouseY;
    };
}