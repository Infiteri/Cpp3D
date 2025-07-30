#pragma once

#include "Base.h"
#include "Physics/Body/Config.h"
#include "Physics/Body/PhysicsBody.h"
#include "Physics/Body/RigidBody.h"
#include "Physics/Body/StaticBody.h"
#include <vector>

class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btDispatcher;
class btConstraintSolver;
class btCollisionConfiguration;

namespace Core
{
    class CE_API PhysicsEngine
    {
    public:
        struct State
        {
            btDiscreteDynamicsWorld *World = nullptr;
            btBroadphaseInterface *Broadphase = nullptr;
            btDispatcher *Dispatcher = nullptr;
            btConstraintSolver *Solver = nullptr;
            btCollisionConfiguration *CollisionConfig = nullptr;

            std::vector<PhysicsBody *> Bodies;
        };

    public:
        static void Init();
        static void Shutdown();

        static RigidBody *CreateRigidBody(RigidBodyConfig &config);
        static StaticBody *CreateStaticBody(StaticBodyConfig &config);

        static void StartRuntime();
        static void UpdateRuntime();
        static void StopRuntime();
    };
} // namespace Core
