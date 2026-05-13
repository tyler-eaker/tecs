#pragma once
#include "Event.h"

namespace Core {
    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(int keycode) : m_KeyCode(keycode) {}
        int GetKeyCode() const { return m_KeyCode; }

        static EventType GetStaticType() { return EventType::KeyPressed; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "KeyPressed"; }
        virtual int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }
    private:
        int m_KeyCode;
    };
}