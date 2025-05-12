#pragma once

#include "Base.h"
#include "Core/Event/Event.h"
#include <vector>

namespace Core
{
    class CE_API EventCustomTypeDestroyer
    {
    public:
        EventCustomTypeDestroyer() {};
        virtual ~EventCustomTypeDestroyer() {};

        // note: It returns true if the event is destroyed, else it returns false. (At least this is
        // how it should be implemented)
        virtual bool DestroyEvent(Event *e) = 0;
    };

    class CE_API EventSystem
    {
    public:
        struct State
        {
            int EventsPerFrame = 10000; // todo: Needed to cap events per frame?
            std::vector<Event *> EventsToHandle;

            // todo: Dispatchers
            EventCustomTypeDestroyer *CustomEventDestroyer = nullptr;
        };

    private:
        static void Emit(Event *event);

    public:
        EventSystem() {};
        ~EventSystem() {};

        static void Init();
        static void Shutdown();
        static void Update();

        static void SetCustomEventDestroyerInstance(EventCustomTypeDestroyer *inst);

        static void Emit(void *context, u32 size, EventType type, int customEventType = -1);

        static void ClearEventContext(Event *event);

        template <typename T, typename... Args>
        static void Emit(EventType type, int customEventType = -1, Args... args)
        {
            Emit(new T(args...), sizeof(T), type, customEventType);
        }
    };
} // namespace Core
