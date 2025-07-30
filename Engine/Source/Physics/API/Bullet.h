#pragma once

#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Physics/Collider/Collider.h"
#include "Scene/Actor.h"

class btTransform;
class btQuaternion;
class btCollisionShape;
class btRigidBody;

namespace Core
{
    namespace BulletAPI
    {
        struct HandleConfig
        {
            Actor *Owner;
            ColliderConfig *Collider;

            bool IsStatic = false;

            float Mass = 0.5;
            float LinearDamping = 0.5;
            float AngularDamping = 0.5;
            float Restitution = 0.5;
            float Friction = 0.5;
        };

        void UpdateTransform(Transform &transform, btTransform &btTrans);
        Vector3 RotationFromQuaternion(const btQuaternion &quat);
        btTransform ComposeTransform(const Vector3 &pos, const Vector3 &rot);
        btQuaternion BtQuatFromRotation(const Vector3 &rot);
        btCollisionShape *CreateShapeFromConfig(ColliderConfig *config);
        btRigidBody *CreateHandle(const HandleConfig &config);
    }; // namespace BulletAPI

} // namespace Core
