#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Types.h"	// Need this for client apps including this file.
#include "core/entitysystem/component/Component.h"

namespace loki
{

class Transform	: public Component
{
	CONTAINER_MACRO_LIST(Transform*, Children);
public:
	DECLARE_COMPONENT_TYPEINFO(Transform)

	Transform();
	~Transform();

	const quat& GetOrientation();
	const vec3& GetPosition();
	const mat4& GetMatrix();

	vec3 GetOrientationVector();

	vec3 GetEulerAngles();
	f32 GetPitch();
	f32 GetYaw();
	f32 GetRoll();

	void SetOrientation( const quat& _Orientation );
	void SetPosition( const vec3& _Position );
	void SetMatrix( const mat4& _Matrix );

	void LocalTranslate( const vec3& _Translation );
	void Translate( const vec3& _Translation );

	// Rotate around local axes.
	void LocalRotateX( f32 _Angle );
	void LocalRotateY( f32 _Angle );
	void LocalRotateZ( f32 _Angle );
	void LocalRotate( vec3& _Axis, f32 _Angle );

	// Rotate around global axes.
	void RotateX( f32 _Angle );
	void RotateY( f32 _Angle );
	void RotateZ( f32 _Angle );
	void Rotate( vec3& _Axis, f32 _Angle );

	void LookAt( const vec3& _Target );

	const mat4& GetLocalToWorldMatrix();
	const mat4& GetWorldToLocalMatrix();

	bool operator == ( Transform& _Transform );
	bool operator != ( Transform& _Transform );
private:
	void _Init();
	void _Terminate();

	void _ComputeMatrix();
	void _ComputePositionAndOrientation();
	void _ComputeOrientation();
	void _ComputePosition();
	void _ComputeWorldToLocalMatrix();

	quat m_Orientation;
	vec3 m_Position;
	mat4 m_Transformation;

	mat4 m_LocalToWorldMatrix;
	mat4 m_WorldToLocalMatrix;

	bool m_MatrixIsDirty;
	bool m_PositionAndOrientationIsDirty;

	bool m_WorldToLocalMatrixIsDitry;
};

}

#endif