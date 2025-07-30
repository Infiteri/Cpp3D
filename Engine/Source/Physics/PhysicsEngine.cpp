#include "PhysicsEngine.h"

#include "Core/Engine.h"
#include "Physics/Body/Config.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <memory>

namespace Core
{
    static PhysicsEngine::State state;

    void PhysicsEngine::Init()
    {
        state.CollisionConfig = new btDefaultCollisionConfiguration();
        state.Dispatcher = new btCollisionDispatcher(state.CollisionConfig);
        state.Broadphase = new btDbvtBroadphase();
        state.Solver = new btSequentialImpulseConstraintSolver();

        state.World = new btDiscreteDynamicsWorld(state.Dispatcher, state.Broadphase, state.Solver,
                                                  state.CollisionConfig);
        state.World->setGravity(btVector3(0, -9.81f, 0));
    }

    void PhysicsEngine::Shutdown()
    {
        delete state.World;
        delete state.Solver;
        delete state.Broadphase;
        delete state.Dispatcher;
        delete state.CollisionConfig;

        state.World = nullptr;
        state.Solver = nullptr;
        state.Broadphase = nullptr;
        state.Dispatcher = nullptr;
        state.CollisionConfig = nullptr;
    }

    RigidBody *PhysicsEngine::CreateRigidBody(RigidBodyConfig &config)
    {
        auto p = new RigidBody(config);
        state.Bodies.push_back(p);
        state.World->addRigidBody(p->handle);
        return p;
    }

    StaticBody *PhysicsEngine::CreateStaticBody(StaticBodyConfig &config)
    {
        auto p = new StaticBody(config);
        state.Bodies.push_back(p);
        state.World->addRigidBody(p->handle);
        return p;
    }

    void PhysicsEngine::StartRuntime() {}

    void PhysicsEngine::UpdateRuntime()
    {
        state.World->stepSimulation(Engine::GetDeltaTime());
        for (auto &b : state.Bodies)
        {
            b->Update();
        }
    }

    void PhysicsEngine::StopRuntime()
    {
        for (auto &p : state.Bodies)
        {
            state.World->removeRigidBody(p->handle);
        }
    }
} // namespace Core
