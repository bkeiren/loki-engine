#include "core/physics/physics.h"
#include "core/physics/rigidbody.h"

#include "core/engine.h"

#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"

#ifdef PHY_DEBUG_DRAW
#include "core/physics/physicsdebugdrawer.h"
#endif

#define VECTOR3	btVector3

namespace loki
{

namespace physics
{

LkPhysics* g_Physics = NULL;

LkPhysics::LkPhysics()	:
	m_FixedTimeStep(1.0f / 60.0f),
	m_WorldZPlane(NULL)
{
	_Init();
}

LkPhysics::~LkPhysics()
{
	_Shutdown();
}

bool LkPhysics::_Init()
{
	m_BroadPhase = new btDbvtBroadphase();
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
	m_SequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();
	m_DynamiscWorld = new btDiscreteDynamicsWorld(m_CollisionDispatcher, m_BroadPhase, m_SequentialImpulseConstraintSolver, m_CollisionConfiguration);

	//////////////////////////////////////////////////////////////////////////
	// Initialize values.
	m_DynamiscWorld->setGravity(VECTOR3(0.0f, -9.81f, 0.0f));


	//////////////////////////////////////////////////////////////////////////
	// Add a floor to the world -- Only for testing?
	RigidBodyInfo info;
	info.m_Mass = 0.0f;
	info.m_Shape = CS_STATICPLANE;
	info.m_StaticPlaneData.m_Constant;
	m_WorldZPlane = AddRigidBody(info);
	m_WorldZPlane->SetPosition(vec3(0.0f, -10.0f, 0.0f));

#ifdef PHY_DEBUG_DRAW
	m_DebugDrawer = new LkDebugDrawer();
	m_DynamiscWorld->setDebugDrawer(m_DebugDrawer);
	m_DebugDawingEnabled = false;
#endif

	LOG(VL_ALWAYS, "Physics::Init: Physics initialized");
	return true;
}

void LkPhysics::_Shutdown()
{
#ifdef PHY_DEBUG_DRAW
	delete m_DebugDrawer;
#endif
	delete m_DynamiscWorld;
	delete m_SequentialImpulseConstraintSolver;
	delete m_CollisionDispatcher;
	delete m_CollisionConfiguration;
	delete m_BroadPhase;

	LOG(VL_ALWAYS, "Physics::Shutdown: Physics terminated");
}

void LkPhysics::_DebugDraw()
{
#ifdef PHY_DEBUG_DRAW
	if (DebugDrawingIsEnabled())
	{
		m_DynamiscWorld->debugDrawWorld();
	}
#endif
}

void LkPhysics::_Update()
{
	m_DynamiscWorld->stepSimulation(g_Engine->GetFrameTime(), 10, m_FixedTimeStep);
}

f32 LkPhysics::GetFixedTimeStep() const
{
	return m_FixedTimeStep;
}

void LkPhysics::SetFixedTimeStep( f32 _TimeStep )
{
	m_FixedTimeStep = _TimeStep;
}

void LkPhysics::SetGravity( const vec3& _Gravity )
{
	m_DynamiscWorld->setGravity(BTVec3(_Gravity));
}

vec3 LkPhysics::GetGravity() const
{
	return GLMVec3(m_DynamiscWorld->getGravity());
}

LkRigidBody* LkPhysics::AddRigidBody( const RigidBodyInfo& _Info )
{
	LkRigidBody* body = new LkRigidBody(_Info);
	m_DynamiscWorld->addRigidBody(body->m_RigidBody);
	return body;
}

void LkPhysics::RemoveRigidBody( LkRigidBody* _Body )
{
	m_DynamiscWorld->removeRigidBody(_Body->m_RigidBody);
	delete _Body;
}

void LkPhysics::SetDebugDrawingEnabled( bool _Enabled )
{
#ifdef PHY_DEBUG_DRAW
	m_DebugDawingEnabled = _Enabled;
#endif
}

bool LkPhysics::DebugDrawingIsEnabled() const
{
#ifdef PHY_DEBUG_DRAW
	return m_DebugDawingEnabled;
#else
	return false;
#endif
}

bool LkPhysics::DebugDrawingWasCompiled() const
{
#ifdef PHY_DEBUG_DRAW
	return true;
#else
	return false;
#endif
}

}

}

#ifdef VECTOR3
	#undef VECTOR3
#endif