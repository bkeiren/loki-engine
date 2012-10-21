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
	friend class Entity;
public:
	bool CreateBodyFromInfo( physics::RigidBodyInfo& _Info );
	
	physics::LkRigidBody* GetBody() const;

protected:
	PhysicsComponent();
	~PhysicsComponent();

private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();

	physics::LkRigidBody* m_RigidBody;
};

}

}

#endif