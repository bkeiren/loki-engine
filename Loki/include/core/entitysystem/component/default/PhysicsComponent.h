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
public:
	DECLARE_COMPONENT_TYPEINFO(PhysicsComponent)	// Required!

	PhysicsComponent();
	~PhysicsComponent();

	bool CreateBodyFromInfo( physics::RigidBodyInfo& _Info );
	
	physics::LkRigidBody* GetBody() const;
private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();

	physics::LkRigidBody* m_RigidBody;
};

}

}

#endif