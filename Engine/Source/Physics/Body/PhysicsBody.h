#pragma once

#include "Base.h"

class btRigidBody;

namespace Core
{
    class PhysicsBody
    {
    protected:
        btRigidBody *handle;
        friend class PhysicsEngine;

    public:
        PhysicsBody() {};
        ~PhysicsBody() {};

        virtual void Update() {};

        template <typename T> T *As() { return (T *)this; };
    };
} // namespace Core
