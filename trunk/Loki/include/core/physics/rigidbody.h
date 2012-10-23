#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "core/physics/rigidbodyinfo.h"

class btRigidBody;

namespace loki
{

namespace physics
{

class LkRigidBody
{
	friend class LkPhysics;
public:
	void SetPosition( const vec3& _Position, bool _PreserveForces = false );
	vec3 GetPosition() const;

	void SetOrientation( const quat& _Orientation, bool _PreserveForces = false );

	void SetTransformation( const mat4& _Transformation, bool _PreserveForces = false );

	void ApplyCentralForce( const vec3& _Force );
	void ApplyCentralImpulse( const vec3& _Impulse );
	void ApplyDamping( f32 _TimeStep );
	void ApplyForce( const vec3& _Force, const vec3& _Point );
	void ApplyGravity();
	void ApplyImpulse( const vec3& _Impulse, const vec3& _Point );
	void ApplyTorque( const vec3& _Torque );
	void ApplyTorqueImpulse( const vec3& _Torque );

	void ClearForces();

	void GetAABB( vec3& _AABBMin, vec3& _AABBMax ) const;
	vec3 GetCenterOfMass() const;
	vec3 GetDeltaAngularVelocity() const;
	vec3 GetDeltaLinearVelocity() const;
	f32 GetFriction() const;
	vec3 GetGravity() const;
	f32 GetLinearDamping() const;
	vec3  GetLinearFactor() const;
	vec3 GetLinearVelocity() const;
	quat GetOrientation() const;
	f32 GetRestitution() const;
	vec3 GetTotalForce() const;
	vec3 GetTotalTorque() const;
	vec3 GetVelocityInLocalPoint( const vec3& _Point ) const;
	mat4 GetWorldTransform() const;
	void Translate( const vec3& _Translation );
	bool IsActive() const;
	void Activate( bool _ForceActivation = false );

	void SetMass( f32 _Mass );
private:
	LkRigidBody( const RigidBodyInfo& _Info );
	LkRigidBody();
	~LkRigidBody();

	btRigidBody* m_RigidBody;
	ECollisionShape m_Shape;
};

}

}

#endif