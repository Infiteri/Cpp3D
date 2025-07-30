#include "StaticBody.h"
#include "Physics/API/Bullet.h"

#include "Scene/Actor.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Core
{
    StaticBody::StaticBody(const StaticBodyConfig &bodyConfig)
    {
        config = bodyConfig;
        handle = nullptr;

        _CreateHandle();
    }

    StaticBody::~StaticBody()
    {
        if (handle)
        {
            delete handle->getMotionState();
            delete handle->getCollisionShape();
            delete handle;
        }

        handle = nullptr;
    }

    void StaticBody::_CreateHandle()
    {
        if (handle)
        {
            delete handle->getMotionState();
            delete handle->getCollisionShape();
            delete handle;
        }

        BulletAPI::HandleConfig hConfig;
        hConfig.IsStatic = true;
        hConfig.Mass = 0;
        hConfig.LinearDamping = config.LinearDamping;
        hConfig.AngularDamping = config.AngularDamping;
        hConfig.Friction = config.Friction;
        hConfig.Restitution = config.Restitution;
        hConfig.Collider = &config.ColliderConfig;
        hConfig.Owner = config.Owner;
        handle = BulletAPI::CreateHandle(hConfig);
    }

    void StaticBody::Update()
    {
        BulletAPI::UpdateTransform(config.Owner->GetTransform(), handle->getWorldTransform());
    }

} // namespace Core
