#pragma once

#include "Physics/Collider/Collider.h"

namespace Core
{
    struct PhysicsBodyConfig
    {
        class Actor *Owner;

        ColliderConfig ColliderConfig;

        float LinearDamping = 0.5;
        float AngularDamping = 0.5;
        float Restitution = 0.5;
        float Friction = 0.5;
    };

    struct RigidBodyConfig : PhysicsBodyConfig
    {
        float Mass = 0.5;
    };

    struct StaticBodyConfig : PhysicsBodyConfig
    {
    };
} // namespace Core
