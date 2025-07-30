#pragma once

#include "Physics/Body/Config.h"
#include "Physics/Body/PhysicsBody.h"

#include "Base.h"

class btRigidBody;

namespace Core
{
    class CE_API RigidBody : public PhysicsBody
    {
    private:
        RigidBodyConfig config;

        friend class PhysicsEngine;

        void _CreateHandle();

    public:
        RigidBody(const RigidBodyConfig &bodyConfig);
        ~RigidBody();

        void Update();

        inline RigidBodyConfig &GetConfig() { return config; };
    };
} // namespace Core
