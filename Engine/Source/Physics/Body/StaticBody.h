#pragma once

#include "Physics/Body/Config.h"
#include "Physics/Body/PhysicsBody.h"

#include "Base.h"

class btRigidBody;

namespace Core
{
    class CE_API StaticBody : public PhysicsBody
    {
    private:
        StaticBodyConfig config;

        friend class PhysicsEngine;

        void _CreateHandle();

    public:
        StaticBody(const StaticBodyConfig &bodyConfig);
        ~StaticBody();

        void Update();

        inline StaticBodyConfig &GetConfig() { return config; };
    };
} // namespace Core
