#include "EventSystem.h"
#include "Core/Event/CeEvents.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Logger.h"

#include <cmath>

#define CE_IMPL_EVENT_DESTROY_VERB(type, cast)                                                     \
    case EventType::type:                                                                          \
    {                                                                                              \
        cast *e = (cast *)event->GetData();                                                        \
        delete e;                                                                                  \
    }                                                                                              \
    break;

#define CE_IMPL_EVENT_DESTROY(t) CE_IMPL_EVENT_DESTROY_VERB(t, Event##t)

namespace Core
{
    static EventSystem::State state;

    void EventSystem::Init() { state.CustomEventDestroyer = nullptr; }

    void EventSystem::Shutdown()
    {
        if (!state.EventsToHandle.empty())
        {
            CE_WARN("Event system has more events to handle, to inspect.");

            for (auto e : state.EventsToHandle)
            {
                ClearEventContext(e);
                delete e;
            }
        }

        state.EventsToHandle.clear();

        if (state.CustomEventDestroyer != nullptr)
            delete state.CustomEventDestroyer;
    }

    void EventSystem::Update()
    {
        if (state.EventsPerFrame == 0 || state.EventsToHandle.empty())
            return;

        int ammount = fmin(state.EventsPerFrame, state.EventsToHandle.size());

        for (int i = 0; i < ammount; i++)
        {
            Event *currentEvent = state.EventsToHandle[i];

            if (currentEvent->GetType() != EventType::Custom)
                currentEvent->customEventType = -1;

            LayerStack::EmitEvent(currentEvent);

            // note: Clears all that is event related here
            if (currentEvent)
            {
                ClearEventContext(currentEvent);
                delete currentEvent;
            }
        }

        state.EventsToHandle.erase(state.EventsToHandle.begin(),
                                   state.EventsToHandle.begin() + ammount);
    }

    void EventSystem::SetCustomEventDestroyerInstance(EventCustomTypeDestroyer *inst)
    {
        CE_VERIFY(inst);

        if (state.CustomEventDestroyer != nullptr)
        {
            CE_ERROR("Custom Event Destoyer already exists");
            delete inst;
        }

        state.CustomEventDestroyer = inst;
    }

    void EventSystem::Emit(Event *event) { state.EventsToHandle.push_back(event); }

    void EventSystem::Emit(void *context, u32 size, EventType type, int customEventType)
    {
        Event *e = new Event();
        e->SetContext(size, context, type);
        e->SetCustomEventType(customEventType);

        Emit(e);
    }

    void EventSystem::ClearEventContext(Event *event)
    {
        switch (event->GetType())
        {
        case EventType::None:
        default:
            CE_WARN("Unknown event type '%i'", event->GetType());
            break;

            CE_IMPL_EVENT_DESTROY(MouseButton);
            CE_IMPL_EVENT_DESTROY(MouseMove);
            CE_IMPL_EVENT_DESTROY(KeyboardButton);
            CE_IMPL_EVENT_DESTROY(Resize);

        case EventType::Custom:
            if (state.CustomEventDestroyer == nullptr)
            {
                CE_ERROR("Unable to destroy custom event as there is no destroyer");
            }
            else
            {
                bool result = state.CustomEventDestroyer->DestroyEvent(event);
                if (!result)
                    CE_ERROR("Custom Event Destroyer returned false, which means that the event "
                             "wasn't properly destroyed.");
            }
            break;
        }
    }

} // namespace Core
