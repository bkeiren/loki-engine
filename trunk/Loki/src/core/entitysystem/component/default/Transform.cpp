#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

Transform::Transform()	:
	m_Orientation(quat()),
	m_Position(vec3(0.0f, 0.0f, 0.0f)),
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
		_ComputePositionAndOrientation();
	}
	return m_Orientation;
}

const vec3& Transform::GetPosition()
{
	if (_IsDirtyFlagSet(DIRTY_FLAG_POS_ORI_SCALE))
	{
		_ComputePositionAndOrientation();
	}
	return m_Position;
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
	}

	m_Position = _Position;
	_SetDirtyFlag(DIRTY_FLAG_MATRIX);
	_ClearDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

void Transform::SetMatrix( const mat4& _Matrix )
{
	m_Transformation = _Matrix;
	_ClearDirtyFlag(DIRTY_FLAG_MATRIX);
	_SetDirtyFlag(DIRTY_FLAG_POS_ORI_SCALE);
	_SetDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
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

void Transform::LookAt( const vec3& _Target )
{
	SetMatrix(math::gtc::matrix_transform::lookAt(GetPosition(), _Target, UP));
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
	_ClearDirtyFlag(DIRTY_FLAG_MATRIX);
}

void Transform::_ComputePositionAndOrientation()
{
	_ComputeOrientation();
	_ComputePosition();
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

void Transform::_ComputeWorldToLocalMatrix()
{
	m_WorldToLocalMatrix = math::inverse(m_Transformation);
	_ClearDirtyFlag(DIRTY_FLAG_WORLD_TO_LOCAL_MATRIX);
}

}