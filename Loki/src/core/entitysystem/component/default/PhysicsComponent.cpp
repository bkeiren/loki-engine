#include "core/entitysystem/component/default/PhysicsComponent.h"
#include "core/entitysystem/component/default/MeshRenderer.h"
#include "core/graphics/Mesh.h"
#include "core/physics/physics.h"
#include "core/graphics/Model.h"
#include "core/entitysystem/Entity.h"

namespace loki
{

namespace components
{

PhysicsComponent::PhysicsComponent()	:
	m_RigidBody(0)
{
	SubscribeToEvent(EVENT_PREPHYSICSUPDATE);
	SubscribeToEvent(EVENT_POSTPHYSICSUPDATE);
}

PhysicsComponent::~PhysicsComponent()
{
	UnsubscribeFromEvent(EVENT_PREPHYSICSUPDATE);
	UnsubscribeFromEvent(EVENT_POSTPHYSICSUPDATE);
}

bool PhysicsComponent::CreateBodyFromInfo( physics::RigidBodyInfo& _Info )
{
	if (m_RigidBody)
	{
		physics::g_Physics->RemoveRigidBody(m_RigidBody);
	}
	m_RigidBody = physics::g_Physics->AddRigidBody(_Info);

	return (m_RigidBody != 0);
}

physics::LkRigidBody* PhysicsComponent::GetBody() const
{
	return m_RigidBody;
}

void PhysicsComponent::_HandleEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
 	{
 	case EVENT_PREPHYSICSUPDATE:
 		{
 			// This is hit right before the physics simulation is stepped.
 			// The user might have positioned objects manually, so we need to sync physics with the user's transformations.
 			m_RigidBody->SetTransformation(GetEntity()->GetTransform().GetMatrix());
 
 			break;
 		}
 	case EVENT_POSTPHYSICSUPDATE:
 		{
 			// This is hit right after the physics simulation is stepped.
 			// We now need to sync movable components with physics components in order to let the visual aspect of an actor
 			// line up with the physics aspect.
			GetEntity()->GetTransform().SetMatrix(m_RigidBody->GetWorldTransform());
 
 			break;
 		}
 	}
}

void PhysicsComponent::_Init()
{
	// By default, the physics component will check for a MeshRenderer and sync with its mesh.
	// Alterations can be made afterwards.
	// if there is no render component, the shape of the physicsshape is a simple box of unit size.
	  
	components::MeshRenderer* meshrenderer = GetEntity()->GetComponent<components::MeshRenderer>();

   	physics::RigidBodyInfo info;

   	// Check whether we have a render component, a model on that component and a mesh on that model.
   	if (meshrenderer)
   	{
		const graphics::Mesh* mesh = meshrenderer->GetMesh();
		const graphics::SubMesh* submesh = mesh->GetSubMesh(0);

   		info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
		info.m_MeshData.m_Mesh = const_cast<graphics::SubMesh*>(submesh);
   	}
   	else
   	{
   		// Use a simple unit box.
   		info.m_Shape = physics::CS_BOX;
   		info.m_BoxData.m_HalfExtents = vec3(0.5f, 0.5f, 0.5f);
   	}
   
   	info.m_InitialTransform = GetEntity()->GetTransform().GetMatrix();
   
   	m_RigidBody = physics::g_Physics->AddRigidBody(info);
}

void PhysicsComponent::_Terminate()
{
	if (m_RigidBody)
	{
		physics::g_Physics->RemoveRigidBody(m_RigidBody);
	}
}

}

}