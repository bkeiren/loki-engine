#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

// Must be located at the top of the file.
#include "core/physics/physics.inl"

#include "core/physics/rigidbody.h"

#define PHY_DEBUG_DRAW

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace loki
{

namespace physics
{

#ifdef PHY_DEBUG_DRAW
class LkDebugDrawer;
#endif

class LkRigidBody;

class LkPhysics
{
	friend class LkEngine;
public:
	void Update();

#ifdef PHY_DEBUG_DRAW
	void DebugDraw();
#endif

	float GetFixedTimeStep() const;
	void SetFixedTimeStep( float _TimeStep );

	void SetGravity( const vec3& _Gravity );
	vec3 GetGravity() const;

	LkRigidBody* AddRigidBody( const RigidBodyInfo& _Info );
	void RemoveRigidBody( LkRigidBody* _Body );
private:
	LkPhysics();
	~LkPhysics();

	bool _Init();
	void _Shutdown();

	//////////////////////////////////////////////////////////////////////////
	btBroadphaseInterface* m_BroadPhase;
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_CollisionDispatcher;
	btSequentialImpulseConstraintSolver* m_SequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* m_DynamiscWorld;

	float m_FixedTimeStep;

	LkRigidBody* m_WorldZPlane;

#ifdef PHY_DEBUG_DRAW
	LkDebugDrawer* m_DebugDrawer;
#endif
};

extern LkPhysics* g_Physics;

}

}

#endif