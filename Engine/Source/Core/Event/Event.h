#pragma once

#include "Base.h"

namespace Core
{
    enum class EventType
    {
        None = 0,
        Custom,
        MouseButton,
        MouseMove,
        KeyboardButton,
        Resize
    };

    class CE_API Event
    {
    private:
        EventType type = EventType::None;
        void *data = nullptr;
        u32 dataSize = 0;
        int customEventType = -1;

        friend class EventSystem;

    public:
        Event();
        Event(EventType type);

        // note: The event system should take care of destroying context
        ~Event() = default;

        void SetContext(u32 dataSize, void *data, EventType type, bool clearOldContext = true);
        void SetCustomEventType(int type);

        inline int GetCustomEventType()
        {
            return (type == EventType::Custom) ? customEventType : -1;
        };

        inline EventType GetType() { return type; };

        inline u32 GetDataSize() { return dataSize; };

        inline void *GetData() { return data; };

        template <typename T> T *GetData() { return (T *)data; };
    };
} // namespace Core
