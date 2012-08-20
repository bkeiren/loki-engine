#pragma once

#ifndef PHYSICS_INL
#define PHYSICS_INL

#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"

namespace loki
{

namespace physics
{

inline btVector3 BTVec3( const glm::vec3& _GLMVec )
{
	return btVector3(_GLMVec.x, _GLMVec.y, _GLMVec.z);
}

inline btQuaternion BTQuat( const glm::quat& _GLMQuat )
{
	return btQuaternion(_GLMQuat.x, _GLMQuat.y, _GLMQuat.y, _GLMQuat.w);
}

inline btTransform BTTransform( const glm::mat4& _GLMMat )
{
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(_GLMMat));
	return trans;
}

inline glm::vec3 GLMVec3( const btVector3& _BTVec )
{
	return glm::vec3(_BTVec.x(), _BTVec.y(), _BTVec.z());
}

inline glm::quat GLMQuat( const btQuaternion& _BTQuat )
{
	return glm::quat(_BTQuat.x(), _BTQuat.y(), _BTQuat.z(), _BTQuat.w());
}

inline glm::mat4 GLMMat( const btTransform& _BTTransform )
{
	float m[16];
	_BTTransform.getOpenGLMatrix(m);
	return glm::mat4(m[0], m[1], m[2], m[3],
					 m[4], m[5], m[6], m[7],
					 m[8], m[9], m[10], m[11],
					 m[12], m[13], m[14], 1.0f);
	
}

}

}

#endif