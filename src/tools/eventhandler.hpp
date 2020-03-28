#pragma once

#include "tools/serializer/serializer.hpp"

#include <functional>

class EventHandler
{
public:
    using EventFunc = std::function<void(void *target, DeSerializer &param)>;

    struct EventItem
    {
        bool operator<(const EventItem &rhs) const { return target < rhs.target; }
        void *target;
        EventFunc f;
    };

    static EventHandler &Instance()
    {
        static EventHandler inst;
        return inst;
    }

    bool RegisterEvent(const char *name, const EventItem &event_item)
    {
        m_event_funs[name].insert(event_item);
        return true;
    }

    void UnRegisterEvent(const char *name, void *target)
    {
        m_event_funs[name].erase( { target, nullptr } );
    }

    void Dispatch(const char *name)
    {
        DeSerializer ds(m_s.Ptr(), m_s.Size());
        m_s.ResetPos(); 

        auto it = m_event_funs.find(name);
        if (it != m_event_funs.end())
        {
            for (auto &event_item : it->second)
                event_item.f(event_item.target, ds);
        }
    }

    template<typename T, typename... Args>
    void Dispatch(const char *name, const T &v, const Args &...rest)
    {
        m_s << v;
        this->Dispatch(name, rest...);
    }

private:
    EventHandler() {}

    std::map<std::string, std::set<EventItem>> m_event_funs;
    Serializer m_s;
};
