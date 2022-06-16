#pragma once

#include "rbpch.h"
#include "Rabbit/Core/Core.h"

namespace Rabbit {

    // Events in Hazel are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication	= BIT(0),
        EventCategoryInput			= BIT(1),
        EventCategoryKeyboard		= BIT(2),
        EventCategoryMouse			= BIT(3),
        EventCategoryMouseButton	= BIT(4)
    };

/* 
    ##: 代表原始英文，   即 KeyPressed;
     #：代表英文字符串， 即 "KeyPressed"

    GetStaticType(): 每个派生的Event类都有自己所独有的type类型，这个接口就是为了获取类的Event类型
    GetEventType() : 这个接口是为了当父类Event对象指向子类对象时，判断对象具体所属的子类类型所用

*/
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class RABBIT_API Event
    {
    public:
        bool Handled = false;
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {

    public:
        EventDispatcher(Event& e) : m_Event(e) {}

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }


    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}
