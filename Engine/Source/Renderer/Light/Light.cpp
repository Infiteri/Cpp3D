#include "Light.h"

namespace Core
{
    static LightIDManager::State state;

    void LightIDManager::BeginFrame()
    {
        state.PointLastFrameCount = state.PointLightID;
        state.PointLightID = 0;

        state.SpotLastFrameCount = state.SpotLightID;
        state.SpotLightID = 0;
    }

    u32 LightIDManager::PointGetLastFrameCount() { return state.PointLastFrameCount; }

    u32 LightIDManager::PointGetID() { return state.PointLightID++; };

    u32 LightIDManager::SpotGetLastFrameCount() { return state.SpotLastFrameCount; }

    u32 LightIDManager::SpotGetID() { return state.SpotLightID++; };
} // namespace Core
