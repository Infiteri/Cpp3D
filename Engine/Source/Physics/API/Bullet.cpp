#include "Bullet.h"

#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Physics/Collider/Collider.h"
#include "Platform/Platform.h"

#include <btBulletDynamicsCommon.h>

namespace Core
{
    namespace BulletAPI
    {
        inline float NormalizeAngle(float angle)
        {
            while (angle > 180.0f)
                angle -= 360.0f;
            while (angle < -180.0f)
                angle += 360.0f;
            return angle;
        }

        void UpdateTransform(Transform &transform, btTransform &btTrans)
        {
            transform.Position.x = btTrans.getOrigin().x();
            transform.Position.y = btTrans.getOrigin().y();
            transform.Position.z = btTrans.getOrigin().z();

            btQuaternion currentRotation = btTrans.getRotation();
            Vector3 euler = RotationFromQuaternion(currentRotation);
            transform.Rotation.x = NormalizeAngle(euler.x);
            transform.Rotation.y = NormalizeAngle(euler.y);
            transform.Rotation.z = NormalizeAngle(euler.z);
        }

        Vector3 RotationFromQuaternion(const btQuaternion &quat)
        {
            btScalar roll, pitch, yaw; // Rotation around Z, Y, and X axes respectively
            quat.getEulerZYX(roll, yaw, pitch);

            return Vector3(pitch * CE_RAD_TO_DEG, yaw * CE_RAD_TO_DEG, roll * CE_RAD_TO_DEG);
        }

        btQuaternion BtQuatFromRotation(const Vector3 &rot)
        {
            btQuaternion q;
            q.setEulerZYX(rot.z * CE_DEG_TO_RAD, rot.y * CE_DEG_TO_RAD, rot.x * CE_DEG_TO_RAD);
            return q;
        }

        btTransform ComposeTransform(const Vector3 &pos, const Vector3 &rot)
        {
            btTransform t;
            btVector3 bPos = {pos.x, pos.y, pos.z};
            btQuaternion bQuat = BtQuatFromRotation(rot);

            t.setIdentity();
            t.setOrigin(bPos);
            t.setRotation(bQuat);

            return t;
        }

        btCollisionShape *CreateShapeFromConfig(ColliderConfig *config)
        {
            btCollisionShape *shape;

            switch (config->Type)
            {
            case ColliderType::Box:
                shape = new btBoxShape(
                    {config->BoxType.Width, config->BoxType.Height, config->BoxType.Depth});
                break;

            default:
                CE_WARN("Unable to create bullet shape from config, unknown type");
                shape = new btBoxShape({1, 1, 1});
                break;
            }

            return shape;
        }

        btRigidBody *CreateHandle(const HandleConfig &config)
        {
            btCollisionShape *shape = BulletAPI::CreateShapeFromConfig(config.Collider);
            btTransform transform = BulletAPI::ComposeTransform(
                config.Owner->GetTransform().Position, config.Owner->GetTransform().Rotation);
            btDefaultMotionState *motion = new btDefaultMotionState(transform);

            btVector3 inertia(0, 0, 0);
            if (!config.IsStatic && config.Mass > 0.0f)
            {
                shape->calculateLocalInertia(config.Mass, inertia);
            }

            btRigidBody::btRigidBodyConstructionInfo info(config.IsStatic ? 0.0f : config.Mass,
                                                          motion, shape, inertia);
            btRigidBody *handle = new btRigidBody(info);

            handle->setDamping(config.LinearDamping, config.AngularDamping);
            handle->setFriction(config.Friction);
            handle->setRestitution(config.Restitution);
            handle->activate(true);
            handle->setUserPointer(config.Owner);

            return handle;
        }
    } // namespace BulletAPI
} // namespace Core
