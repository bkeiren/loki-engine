// #pragma once
// 
// #ifndef LKPHYSICSCOMPONENT_H
// #define LKPHYSICSCOMPONENT_H
// 
// #include <list>
// #include "core/actor/components/base/actorcomponent.h"
// #include "core/physics/rigidbodyinfo.h"
// 
// namespace loki
// {
// 
// namespace physics
// {
// 
// class LkRigidBody;
// 
// }
// 
// class LkPhysicsComponent	: public LkActorComponent
// {
// public:
// 	LkPhysicsComponent();
// 	~LkPhysicsComponent();
// 
// 	bool CreateBodyFromInfo( physics::RigidBodyInfo& _Info );
// 
// 	physics::LkRigidBody* GetBody() const;
// protected:
// 	void _OnEvent( const LkEvent& _event );
// 
// private:
// 	void _Init();
// 
// 	physics::LkRigidBody* m_RigidBody;
// };
// 
// }
// 
// #endif