#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Types.h"	// Need this for client apps including this file.
#include "core/entitysystem/component/Component.h"

#define TRANSFORM_SUPPORT_PROPERTIES
#ifdef TRANSFORM_SUPPORT_PROPERTIES
#include "util/property/Property.h"
using namespace loki::util;
#endif

namespace loki
{

class Transform	: public Component
{
	CONTAINER_MACRO_LIST(Transform*, Children);
public:
	DECLARE_COMPONENT_TYPEINFO(Transform)

	const quat& GetOrientation();
	const vec3& GetPosition();
	const vec3& GetScale();
	const mat4& GetMatrix();

	vec3 GetOrientationVector();

	vec3 GetEulerAngles();
	f32 GetPitch();
	f32 GetYaw();
	f32 GetRoll();

	void SetOrientation( const quat& _Orientation );
	void SetPosition( const vec3& _Position );
	void SetScale( float _Scale );
	void SetScale( const vec3& _Scale );
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

	void Scale( float _Scale );
	void Scale( const vec3& _Scale );

	void LookAt( const vec3& _Target );

	bool ScaleIsUniform();

	const mat4& GetLocalToWorldMatrix();
	const mat4& GetWorldToLocalMatrix();

	bool operator == ( Transform& _Transform );
	bool operator != ( Transform& _Transform );

#ifdef TRANSFORM_SUPPORT_PROPERTIES

	PROPERTY(vec3, Transform, position,
		{
			return self.GetPosition();
		},
		{
			self.SetPosition(value);
		}
	);

	PROPERTY(quat, Transform, orientation,
		{
			return self.GetOrientation();
		},
		{
			self.SetOrientation(value);
		}
		);

	PROPERTY(mat4, Transform, transform,
		{
			return self.GetMatrix();
		},
		{
			self.SetMatrix(value);
		}
	);

#endif
private:
	Transform();
	~Transform();

	enum EDirtyFlags
	{
		DIRTY_FLAG_MATRIX = (1 << 0),
		DIRTY_FLAG_POS_ORI_SCALE = (1 << 1),
		DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX = (1 << 2)
	};

	void _Init();
	void _Terminate();

	void _ComputeMatrix();
	void _ComputePositionOrientationAndScale();
	void _ComputeOrientation();
	void _ComputePosition();
	void _ComputeScale();
	void _ComputeWorldToLocalMatrix();

	inline bool _IsDirtyFlagSet( EDirtyFlags _Flag )
	{
		return (m_DirtyFlags & _Flag) == 1;	// Explicit comparison so we don't have those ugly warnings.
	}

	inline void _ClearDirtyFlag( EDirtyFlags _Flag )
	{
		 m_DirtyFlags &= ~_Flag;
	}

	inline void _SetDirtyFlag( EDirtyFlags _Flag )
	{
		m_DirtyFlags |= _Flag;
	}

	quat m_Orientation;
	vec3 m_Position;
	vec3 m_Scale;
	mat4 m_Transformation;

	mat4 m_LocalToWorldMatrix;
	mat4 m_WorldToLocalMatrix;

	char m_DirtyFlags;
};

}

#endif