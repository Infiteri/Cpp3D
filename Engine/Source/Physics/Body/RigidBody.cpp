#include "RigidBody.h"
#include "Physics/API/Bullet.h"

#include "Scene/Actor.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Core
{
    RigidBody::RigidBody(const RigidBodyConfig &bodyConfig)
    {
        config = bodyConfig;
        handle = nullptr;

        _CreateHandle();
    }

    RigidBody::~RigidBody()
    {
        if (handle)
        {
            delete handle->getMotionState();
            delete handle->getCollisionShape();
            delete handle;
        }

        handle = nullptr;
    }

    void RigidBody::_CreateHandle()
    {
        if (handle)
        {
            delete handle->getMotionState();
            delete handle->getCollisionShape();
            delete handle;
        }

        BulletAPI::HandleConfig hConfig;
        hConfig.IsStatic = false;
        hConfig.Mass = config.Mass;
        hConfig.LinearDamping = config.LinearDamping;
        hConfig.AngularDamping = config.AngularDamping;
        hConfig.Friction = config.Friction;
        hConfig.Restitution = config.Restitution;
        hConfig.Collider = &config.ColliderConfig;
        hConfig.Owner = config.Owner;
        handle = BulletAPI::CreateHandle(hConfig);
    }

    void RigidBody::Update()
    {
        BulletAPI::UpdateTransform(config.Owner->GetTransform(), handle->getWorldTransform());
    }
} // namespace Core
