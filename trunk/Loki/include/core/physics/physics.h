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
	friend class LokiEngine;
public:
	f32 GetFixedTimeStep() const;
	void SetFixedTimeStep( f32 _TimeStep );

	void SetGravity( const vec3& _Gravity );
	vec3 GetGravity() const;

	LkRigidBody* AddRigidBody( const RigidBodyInfo& _Info );
	void RemoveRigidBody( LkRigidBody* _Body );

	void SetDebugDrawingEnabled( bool _Enabled );
	bool DebugDrawingIsEnabled() const;
	bool DebugDrawingWasCompiled() const;
private:
	LkPhysics();
	~LkPhysics();
	
	// NOTE: This function only works if:
	// PHY_DEBUG_DRAW was defined at compile time (This compiles all code required to actually be able to debug draw),
	// and if m_DebugDrawingEnabled is set to true. (This can be changed by calling SetDebugDrawingEnabled() and queried
	// by calling DebugDrawingIsEnabled().
	void _DebugDraw();

	void _Update();

	bool _Init();
	void _Shutdown();

	//////////////////////////////////////////////////////////////////////////
	btBroadphaseInterface* m_BroadPhase;
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_CollisionDispatcher;
	btSequentialImpulseConstraintSolver* m_SequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* m_DynamiscWorld;

	f32 m_FixedTimeStep;

	LkRigidBody* m_WorldZPlane;

#ifdef PHY_DEBUG_DRAW
	LkDebugDrawer* m_DebugDrawer;
	bool m_DebugDawingEnabled;
#endif
};

extern LkPhysics* g_Physics;

}

}

#endif