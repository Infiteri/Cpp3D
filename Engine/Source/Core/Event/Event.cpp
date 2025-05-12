#include "Event.h"
#include "Core/Event/EventSystem.h"

namespace Core
{
    Event::Event() : type(EventType::None), data(nullptr), dataSize(0) {}

    Event::Event(EventType type) : type(type), data(nullptr), dataSize(0) {}

    void Event::SetContext(u32 dataSize, void *data, EventType type, bool clearOldContext)
    {
        if (clearOldContext && this->type != EventType::None)
            EventSystem::ClearEventContext(this);

        this->dataSize = dataSize;
        this->data = data;
        this->type = type;

        if (type != EventType::Custom)
            customEventType = -1;
    }

    void Event::SetCustomEventType(int type) { customEventType = type; }
} // namespace Core
