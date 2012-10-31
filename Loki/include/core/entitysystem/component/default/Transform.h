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

	const quat& GetOrientation() const;
	const quat& GetLocalOrientation() const;
	const vec3& GetPosition() const;
	const vec3& GetLocalPosition() const;
	const mat4& GetMatrix();
	const mat4& GetLocalMatrix();

	vec3 GetOrientationVector() const;
	vec3 GetLocalOrientationVector() const;

	vec3 GetEulerAngles() const;
	vec3 GetLocalEulerAngles() const;
	f32 GetPitch() const;
	f32 GetLocalPitch() const;
	f32 GetYaw() const;
	f32 GetLocalYaw() const;
	f32 GetRoll() const;
	f32 GetLocalRoll() const;

	void SetOrientation( const quat& _Orientation );
	void SetLocalOrientation( const quat& _Orientation );
	void SetPosition( const vec3& _Position );
	void SetLocalPosition( const vec3& _Position );
	void SetMatrix( const mat4& _Matrix );
	void SetLocalMatrix( const mat4& _Matrix );

	void LocalTranslate( const vec3& _Position );
	void Translate( const vec3& _Position );
	void LocalRotateX( f32 _Angle );
	void LocalRotateY( f32 _Angle );
	void LocalRotateZ( f32 _Angle );
	void LocalRotate( vec3& _Axis, f32 _Angle );
	void RotateX( f32 _Angle );
	void RotateY( f32 _Angle );
	void RotateZ( f32 _Angle );
	void Rotate( vec3& _Axis, f32 _Angle );

	void LookAt( const vec3& _Target );

	//////////////////////////////////////////////////////////////////////////
	// Sets the parent transform of this transform.
	// This function preserves the world-space transformation,
	// but changes the parent-relative transformation.
	//////////////////////////////////////////////////////////////////////////
	void SetParent( Transform& _Transform );
	Transform& GetParent() const;

	uint32 GetNumChildren() const;

	const mat4& GetLocalToWorldMatrix();
	const mat4& GetWorldToLocalMatrix();

	bool operator == ( Transform& _Transform );
	bool operator != ( Transform& _Transform );
private:
	void _Init();
	void _Terminate();

	void _ComputeMatrix();
	void _ComputeLocalMatrix();

	void _ComputeOrientationFromLocalOrientation();
	void _ComputeLocalOrientationFromOrientation();
	void _ComputePositionFromLocalPosition();
	void _ComputeLocalPositionFromPosition();

	mat4 _ComputeLocalToWorldMatrix();
	mat4 _ComputeWorldToLocalMatrix();

	void _UpdateChildrenLocalOrientation();
	void _UpdateChildrenOrientation();
	void _UpdateChildrenLocalPosition();
	void _UpdateChildrenPosition();

	void _AddChild( Transform* _Child );
	void _RemoveChild( Transform* _Child );

	quat m_LocalOrientation;
	vec3 m_LocalPosition;

	quat m_Orientation;
	vec3 m_Position;

	mat4 m_LocalToWorldMatrix;
	mat4 m_WorldToLocalMatrix;

	mat4 m_LocalTransformation;
	mat4 m_Transformation;

	Transform* m_Parent;
	Children m_Children;

	bool m_MatrixIsDirty;
	bool m_LocalMatrixIsDirty;
	bool m_WorldToLocalMatrixIsDitry;
};

}

#endif