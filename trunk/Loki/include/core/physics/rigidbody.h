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
	void SetPosition( const glm::vec3& _Position, bool _PreserveForces = false );
	glm::vec3 GetPosition() const;

	void ApplyCentralForce( const glm::vec3& _Force );
	void ApplyCentralImpulse( const glm::vec3& _Impulse );
	void ApplyDamping( float _TimeStep );
	void ApplyForce( const glm::vec3& _Force, const glm::vec3& _Point );
	void ApplyGravity();
	void ApplyImpulse( const glm::vec3& _Impulse, const glm::vec3& _Point );
	void ApplyTorque( const glm::vec3& _Torque );
	void ApplyTorqueImpulse( const glm::vec3& _Torque );

	void ClearForces();

	void GetAABB( glm::vec3& _AABBMin, glm::vec3& _AABBMax ) const;
	glm::vec3 GetCenterOfMass() const;
	glm::vec3 GetDeltaAngularVelocity() const;
	glm::vec3 GetDeltaLinearVelocity() const;
	float GetFriction() const;
	glm::vec3 GetGravity() const;
	float GetLinearDamping() const;
	glm::vec3  GetLinearFactor() const;
	glm::vec3 GetLinearVelocity() const;
	glm::quat GetOrientation() const;
	float GetRestitution() const;
	glm::vec3 GetTotalForce() const;
	glm::vec3 GetTotalTorque() const;
	glm::vec3 GetVelocityInLocalPoint( const glm::vec3& _Point ) const;
	glm::mat4 GetWorldTransform() const;
	void Translate( const glm::vec3& _Translation );
	bool IsActive() const;
	void Activate( bool _ForceActivation = false );
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