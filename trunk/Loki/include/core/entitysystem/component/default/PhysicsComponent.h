#pragma once

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "core/entitysystem/component/Component.h"
#include "core/physics/rigidbodyinfo.h"

namespace loki
{

namespace physics
{
class LkRigidBody;
}

namespace components
{

class PhysicsComponent	: public Component
{
	friend class ::loki::Entity;
public:
	bool CreateBodyFromInfo( physics::RigidBodyInfo& _Info );
	
	physics::LkRigidBody* GetBody() const;
private:
	PhysicsComponent();
	~PhysicsComponent();

	void _HandleEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();

	physics::LkRigidBody* m_RigidBody;
};

}

}

REGISTER_COMPONENT(PhysicsComponent)
COMPONENT_SINGLE_INSTANCE(PhysicsComponent)

#endif