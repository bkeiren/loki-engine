#pragma once

#ifndef PHYSICS_INL
#define PHYSICS_INL

#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"

namespace loki
{

namespace physics
{

inline btVector3 BTVec3( const vec3& _GLMVec )
{
	return btVector3(_GLMVec.x, _GLMVec.y, _GLMVec.z);
}

inline btQuaternion BTQuat( const quat& _GLMQuat )
{
	return btQuaternion(_GLMQuat.x, _GLMQuat.y, _GLMQuat.y, _GLMQuat.w);
}

inline btTransform BTTransform( const mat4& _GLMMat )
{
	btTransform trans;
	trans.setFromOpenGLMatrix(math::value_ptr(_GLMMat));
	return trans;
}

inline vec3 GLMVec3( const btVector3& _BTVec )
{
	return vec3(_BTVec.x(), _BTVec.y(), _BTVec.z());
}

inline quat GLMQuat( const btQuaternion& _BTQuat )
{
	return quat(_BTQuat.w(), _BTQuat.x(), _BTQuat.y(), _BTQuat.z());
}

inline mat4 GLMMat( const btTransform& _BTTransform )
{
	f32 m[16];
	_BTTransform.getOpenGLMatrix(m);
	return mat4(m[0], m[1], m[2], m[3],
					 m[4], m[5], m[6], m[7],
					 m[8], m[9], m[10], m[11],
					 m[12], m[13], m[14], 1.0f);
	
}

}

}

#endif