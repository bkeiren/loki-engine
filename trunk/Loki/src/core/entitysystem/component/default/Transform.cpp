#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

Transform::Transform()	:
	m_Orientation(quat()),
	m_Position(vec3(0.0f, 0.0f, 0.0f)),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_LocalToWorldMatrix(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
								0.0f, 1.0f, 0.0f, 0.0f, 
								0.0f, 0.0f, 1.0f, 0.0f, 
								0.0f, 0.0, 0.0f, 1.0f)),
	m_WorldToLocalMatrix(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
								0.0f, 1.0f, 0.0f, 0.0f, 
								0.0f, 0.0f, 1.0f, 0.0f, 
								0.0f, 0.0, 0.0f, 1.0f)),
	m_Transformation(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
						   0.0f, 1.0f, 0.0f, 0.0f, 
						   0.0f, 0.0f, 1.0f, 0.0f, 
						   0.0f, 0.0, 0.0f, 1.0f)),
   m_DirtyFlags(0)
{

}

Transform::~Transform()
{

}

const quat& Transform::GetOrientation()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputePositionOrientationAndScale();
	}
	return m_Orientation;
}

const vec3& Transform::GetPosition()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputePositionOrientationAndScale();
	}
	return m_Position;
}

const vec3& Transform::GetScale()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputePositionOrientationAndScale();
	}
	return m_Scale;
}

const mat4& Transform::GetMatrix()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_MATRIX))
	{
		_ComputeMatrix();
	}
	return m_Transformation;
}

vec3 Transform::GetOrientationVector()
{
	return FORWARD * GetOrientation();
}

vec3 Transform::GetEulerAngles()
{
	return math::gtx::quaternion::eulerAngles(GetOrientation());
}

f32 Transform::GetPitch()
{
	return math::gtx::quaternion::pitch(GetOrientation());
}

f32 Transform::GetYaw()
{
	return math::gtx::quaternion::yaw(GetOrientation());
}

f32 Transform::GetRoll()
{
	return math::gtx::quaternion::roll(GetOrientation());
}

void Transform::SetOrientation( const quat& _Orientation )
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputePosition();
		_ComputeScale();
	}

	m_Orientation = _Orientation;
	_SetDirtyFlag(DIRTY_FLAG_MATRIX);
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

void Transform::SetPosition( const vec3& _Position )
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputeOrientation();
		_ComputeScale();
	}

	m_Position = _Position;
	_SetDirtyFlag(DIRTY_FLAG_MATRIX);
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

void Transform::SetScale( float _Scale )
{
	SetScale(vec3(_Scale));
}

void Transform::SetScale( const vec3& _Scale )
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputeOrientation();
		_ComputePosition();
	}

	m_Scale = _Scale;

	_ComputeMatrix();

	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
	_ClearDirtyFlag(DIRTY_FLAG_MATRIX);
	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

void Transform::SetMatrix( const mat4& _Matrix )
{
// 	m_Transformation = _Matrix;
// 	_ClearDirtyFlag(DIRTY_FLAG_MATRIX);
// 	_SetDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
// 	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);

	// Because otherwise stuff doesn't work...
	SetPosition(vec3(_Matrix[3]));
	SetOrientation(math::quat_cast(_Matrix));
}

void Transform::LocalTranslate( const vec3& _Translation )
{
	SetPosition(GetPosition() + (GetOrientation() * _Translation));
}

void Transform::Translate( const vec3& _Translation )
{
	SetPosition(GetPosition() + _Translation);
}

void Transform::LocalRotateX( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalX * GetOrientation()));
}

void Transform::LocalRotateY( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalY * GetOrientation()));
}

void Transform::LocalRotateZ( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalZ * GetOrientation()));
}

void Transform::LocalRotate( vec3& _Axis, f32 _Angle )
{
	const quat& q = GetOrientation();
	SetOrientation(math::gtc::quaternion::rotate(q, _Angle, _Axis * q));
}

void Transform::RotateX( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalX));
}

void Transform::RotateY( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalY));
}

void Transform::RotateZ( f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, GlobalZ));
}

void Transform::Rotate( vec3& _Axis, f32 _Angle )
{
	SetOrientation(math::gtc::quaternion::rotate(GetOrientation(), _Angle, _Axis));
}

void Transform::Scale( float _Scale )
{
	SetScale(GetScale() * _Scale);
}

void Transform::Scale( const vec3& _Scale )
{
	SetScale(GetScale() * _Scale);
}

void Transform::LookAt( const vec3& _Target )
{
	vec3 pos = GetPosition();
	mat4 m = math::inverse(math::lookAt(pos, _Target, UP));
	m[0] = -m[0];
	m[2] = -m[2];
//	SetMatrix(m);
	SetPosition(pos);
	SetOrientation(math::quat_cast(m));
}

bool Transform::ScaleIsUniform()
{
	vec3 s = GetScale();
	return (s.x == s.y) && (s.x == s.z);
}

const mat4& Transform::GetLocalToWorldMatrix()
{
	return m_Transformation;
}

const mat4& Transform::GetWorldToLocalMatrix()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX))
	{
		_ComputeWorldToLocalMatrix();
	}
	return m_WorldToLocalMatrix;
}

bool Transform::operator == ( Transform& _Transform )
{
	return (GetMatrix() == _Transform.GetMatrix());
}

bool Transform::operator != ( Transform& _Transform )
{
	return !(this->operator == (_Transform));
}

void Transform::_Init()
{

}

void Transform::_Terminate()
{

}

void Transform::_ComputeMatrix()
{
	mat4 m = math::gtc::quaternion::mat4_cast(GetOrientation());
	m[3] = vec4(GetPosition(), 1.0);
	m_Transformation = m;
	m_Transformation[0][0] *= m_Scale.x;
	m_Transformation[1][1] *= m_Scale.y;
	m_Transformation[2][2] *= m_Scale.z;
	_ClearDirtyFlag(DIRTY_FLAG_MATRIX);
}

void Transform::_ComputePositionOrientationAndScale()
{
	_ComputeOrientation();
	_ComputePosition();
	_ComputeScale();
}

void Transform::_ComputeOrientation()
{
	m_Position = vec3(m_Transformation[3]);
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
}

void Transform::_ComputePosition()
{
	m_Orientation = math::gtc::quaternion::quat_cast(m_Transformation);
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
}

void Transform::_ComputeScale()
{
	m_Scale = vec3(	math::length(vec3(m_Transformation[0])), 
					math::length(vec3(m_Transformation[1])), 
					math::length(vec3(m_Transformation[2])) );
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
}

void Transform::_ComputeWorldToLocalMatrix()
{
	m_WorldToLocalMatrix = math::inverse(m_Transformation);
	_ClearDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

}