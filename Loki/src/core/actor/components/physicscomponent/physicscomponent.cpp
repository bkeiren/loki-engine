#include "core/actor/components/physicscomponent/physicscomponent.h"
#include "core/actor/actor.h"
//#include "core/renderer/geometry/mesh/mesh.h"
#include "core/physics/physics.h"
#include "core/actor/components/movablecomponent/movablecomponent.h"
#include "core/actor/components/rendercomponent/rendercomponent.h"
//#include "core/renderer/geometry/model/model.h"

namespace loki
{

LkPhysicsComponent::LkPhysicsComponent()	:
	m_RigidBody(NULL)
{
	SubscribeToEvent(EVENT_PREPHYSICSUPDATE);
	SubscribeToEvent(EVENT_POSTPHYSICSUPDATE);
}

LkPhysicsComponent::~LkPhysicsComponent()
{
	UnsubscribeFromEvent(EVENT_PREPHYSICSUPDATE);
	UnsubscribeFromEvent(EVENT_POSTPHYSICSUPDATE);

	if (m_RigidBody)
	{
		physics::g_Physics->RemoveRigidBody(m_RigidBody);
		m_RigidBody = NULL;
	}
}

bool LkPhysicsComponent::CreateBodyFromInfo( physics::RigidBodyInfo& _Info )
{
	if (m_RigidBody)
	{
		physics::g_Physics->RemoveRigidBody(m_RigidBody);
	}
	m_RigidBody = physics::g_Physics->AddRigidBody(_Info);
	
	return (m_RigidBody != 0);
}

physics::LkRigidBody* LkPhysicsComponent::GetBody() const
{
	return m_RigidBody;
}

void LkPhysicsComponent::_OnEvent( const LkEvent& _event )
{
	switch (_event.GetEventType())
	{
	case EVENT_PREPHYSICSUPDATE:
		{
			// This is hit right before the physics simulation is stepped.
			// The user might have positioned objects manually, so we need to sync physics with the user's transformations.
			LkMovableComponent* comp = GetActor()->GetComponent<LkMovableComponent>();
			if (comp)
			{
				m_RigidBody->SetTransformation(comp->GetTransformation());
			}

			break;
		}
	case EVENT_POSTPHYSICSUPDATE:
		{
			// This is hit right after the physics simulation is stepped.
			// We now need to sync movable components with physics components in order to let the visual aspect of an actor
			// line up with the physics aspect.
			LkMovableComponent* comp = GetActor()->GetComponent<LkMovableComponent>();
			if (comp)
			{
				comp->SetTransformation(m_RigidBody->GetWorldTransform());
			}

			break;
		}
	}
}


void LkPhysicsComponent::_Init()
{
	// By default, the physics component will check for a LkMovableComponent and a LkRenderComponent and sync with those.
	// Alterations can be made afterwards.
	// If there is no movable component, the initial transformation will simply be an identity matrix.
	// if there is no render component, the shape of the shape is a simple box of unit size.
// 
// 	LkMovableComponent* movcomp = GetActor()->GetComponent<LkMovableComponent>();
// 	LkRenderComponent* rendercomp = GetActor()->GetComponent<LkRenderComponent>();
// 
// 	physics::RigidBodyInfo info;
// 
// 	// Check whether we have a render component, a model on that component and a mesh on that model.
// 	if (rendercomp && rendercomp->GetModel() && rendercomp->GetModel()->GetMesh())
// 	{
// 		info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
// 		info.m_MeshData.m_Mesh = const_cast<renderer::LkMesh*>(rendercomp->GetModel()->GetMesh());
// 	}
// 	else
// 	{
// 		// Use a simple unit box.
// 		info.m_Shape = physics::CS_BOX;
// 		info.m_BoxData.m_HalfExtents = vec3(0.5f, 0.5f, 0.5f);
// 	}
// 
// 	// Check whether we have a movable component.
// 	if (movcomp)
// 	{
// 		info.m_InitialTransform = movcomp->GetTransformation();
// 	}
// 	else
// 	{
// 		info.m_InitialTransform = mat4(1.0f, 0.0f, 0.0f, 0.0f,
// 			0.0f, 1.0f, 0.0f, 0.0f,
// 			0.0f, 0.0f, 1.0f, 0.0f,
// 			0.0f, 0.0f, 0.0f, 1.0f);
// 	}
// 
// 	m_RigidBody = physics::g_Physics->AddRigidBody(info);
}

}