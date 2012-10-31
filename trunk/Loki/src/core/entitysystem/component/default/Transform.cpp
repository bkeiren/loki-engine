#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

Transform::Transform()	:
	m_LocalOrientation(quat()),
	m_LocalPosition(vec3(0.0f, 0.0f, 0.0f)),
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
	m_LocalTransformation(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
								0.0f, 1.0f, 0.0f, 0.0f, 
								0.0f, 0.0f, 1.0f, 0.0f, 
								0.0f, 0.0, 0.0f, 1.0f)),
	m_Transformation(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
						   0.0f, 1.0f, 0.0f, 0.0f, 
						   0.0f, 0.0f, 1.0f, 0.0f, 
						   0.0f, 0.0, 0.0f, 1.0f)),
   m_Parent(0),
   m_MatrixIsDirty(false),
   m_LocalMatrixIsDirty(false),
   m_WorldToLocalMatrixIsDitry(false)
{

}

Transform::~Transform()
{

}

const quat& Transform::GetOrientation() const
{
	return m_Orientation;
}

const quat& Transform::GetLocalOrientation() const
{
	return m_LocalOrientation;
}

const vec3& Transform::GetPosition() const
{
	return m_Position;
}

const vec3& Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

const mat4& Transform::GetMatrix()
{
	if (m_MatrixIsDirty)
	{
		_ComputeMatrix();
	}
	return m_Transformation;
}

const mat4& Transform::GetLocalMatrix()
{
	if (m_LocalMatrixIsDirty)
	{
		_ComputeLocalMatrix();
	}
	return m_LocalTransformation;
}

vec3 Transform::GetOrientationVector() const
{
	return GetOrientation() * FORWARD;
}

vec3 Transform::GetLocalOrientationVector() const
{
	return GetLocalOrientation() * FORWARD;
}

vec3 Transform::GetEulerAngles() const
{
	return math::gtx::quaternion::eulerAngles(GetOrientation());
}

vec3 Transform::GetLocalEulerAngles() const
{
	return math::gtx::quaternion::eulerAngles(GetLocalOrientation());
}

f32 Transform::GetPitch() const
{
	return math::gtx::quaternion::pitch(GetOrientation());
}

f32 Transform::GetLocalPitch() const
{
	return math::gtx::quaternion::pitch(GetLocalOrientation());
}

f32 Transform::GetYaw() const
{
	return math::gtx::quaternion::yaw(GetOrientation());
}

f32 Transform::GetLocalYaw() const
{
	return math::gtx::quaternion::yaw(GetLocalOrientation());
}

f32 Transform::GetRoll() const
{
	return math::gtx::quaternion::roll(GetOrientation());
}

f32 Transform::GetLocalRoll() const
{
	return math::gtx::quaternion::roll(GetLocalOrientation());
}

void Transform::SetOrientation( const quat& _Orientation )
{
	// Compute new local orientation from global orientation.
	m_Orientation = _Orientation;
	_ComputeLocalOrientationFromOrientation();
	m_MatrixIsDirty = true;
	m_WorldToLocalMatrixIsDitry = true;

	// Update children.
	_UpdateChildrenOrientation();
}

void Transform::SetLocalOrientation( const quat& _Orientation )
{
	// Compute new global orientation from local orientation.
	m_LocalOrientation = _Orientation;
	_ComputeOrientationFromLocalOrientation();
	m_LocalMatrixIsDirty = true;
	m_WorldToLocalMatrixIsDitry = true;

	// Update children.
	_UpdateChildrenLocalOrientation();
}

void Transform::SetPosition( const vec3& _Position )
{
	// Compute new local position from global position.
	m_Position = _Position;
	_ComputeLocalPositionFromPosition();
	m_MatrixIsDirty = true;
	m_WorldToLocalMatrixIsDitry = true;

	// Update children.
	_UpdateChildrenPosition();
}

void Transform::SetLocalPosition( const vec3& _Position )
{
	// Compute new global position from local position.
	m_LocalPosition = _Position;
	_ComputePositionFromLocalPosition();
	m_LocalMatrixIsDirty = true;
	m_WorldToLocalMatrixIsDitry = true;

	// Update children.
	_UpdateChildrenLocalPosition();
}

void Transform::SetMatrix( const mat4& _Matrix )
{
	SetOrientation(math::gtc::quaternion::quat_cast(_Matrix));
	SetPosition(vec3(_Matrix[3]));
}

void Transform::SetLocalMatrix( const mat4& _Matrix )
{
	SetLocalOrientation(math::gtc::quaternion::quat_cast(_Matrix));
	SetLocalPosition(vec3(_Matrix[3]));
}

void Transform::LocalTranslate( const vec3& _Position )
{
	SetLocalPosition(GetLocalPosition() + _Position);
}

void Transform::Translate( const vec3& _Position )
{
	SetPosition(GetPosition() + _Position);
}

void Transform::LocalRotateX( f32 _Angle )
{
	SetLocalOrientation(math::gtc::quaternion::rotate(GetLocalOrientation(), _Angle, GlobalX));
}

void Transform::LocalRotateY( f32 _Angle )
{
	SetLocalOrientation(math::gtc::quaternion::rotate(GetLocalOrientation(), _Angle, GlobalY));
}

void Transform::LocalRotateZ( f32 _Angle )
{
	SetLocalOrientation(math::gtc::quaternion::rotate(GetLocalOrientation(), _Angle, GlobalZ));
}

void Transform::LocalRotate( vec3& _Axis, f32 _Angle )
{
	const quat& q = GetLocalOrientation();
	SetLocalOrientation(math::gtc::quaternion::rotate(q, _Angle, _Axis * q));
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
	const quat& q = GetOrientation();
	SetOrientation(math::gtc::quaternion::rotate(q, _Angle, _Axis * q));
}

void Transform::LookAt( const vec3& _Target )
{
	SetMatrix(math::gtc::matrix_transform::lookAt(GetPosition(), _Target, UP));
}

void Transform::SetParent( Transform& _Transform )
{
	vec3 GlobalPosition = GetPosition();

	if (m_Parent)
	{
		m_Parent->_RemoveChild(this);
	}

	m_Parent = &_Transform;

	SetPosition(GlobalPosition);

	m_Parent->_AddChild(this);
}

Transform& Transform::GetParent() const
{
	return *m_Parent;
}

uint32 Transform::GetNumChildren() const
{
	return m_Children.size();
}

const mat4& Transform::GetLocalToWorldMatrix()
{
	if (m_WorldToLocalMatrixIsDitry)	// Yes, this is correct.
	{
		_ComputeLocalToWorldMatrix();
	}
	return m_LocalToWorldMatrix;
}

const mat4& Transform::GetWorldToLocalMatrix()
{
	if (m_WorldToLocalMatrixIsDitry)
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
	m_MatrixIsDirty = false;
}

void Transform::_ComputeLocalMatrix()
{
	mat4 m = math::gtc::quaternion::mat4_cast(GetLocalOrientation());
	m[3] = vec4(GetLocalPosition(), 1.0);
	m_LocalTransformation = m;
	m_LocalMatrixIsDirty = false;
}

void Transform::_ComputeOrientationFromLocalOrientation()
{
	m_Orientation = math::gtc::quaternion::quat_cast(m_LocalToWorldMatrix * math::gtc::quaternion::mat4_cast(GetLocalOrientation()));
	m_MatrixIsDirty = true;
}

void Transform::_ComputeLocalOrientationFromOrientation()
{
	m_LocalOrientation = math::gtc::quaternion::quat_cast(m_WorldToLocalMatrix * math::gtc::quaternion::mat4_cast(GetOrientation()));
	m_LocalMatrixIsDirty = true;
}

void Transform::_ComputePositionFromLocalPosition()
{
	m_Position = vec3(GetLocalToWorldMatrix() * vec4(GetLocalPosition(), 1.0f));
	m_MatrixIsDirty = true;
}

void Transform::_ComputeLocalPositionFromPosition()
{
	m_LocalPosition = vec3(GetWorldToLocalMatrix() * vec4(GetPosition(), 1.0f));
	m_LocalMatrixIsDirty = true;
}

mat4 Transform::_ComputeLocalToWorldMatrix()
{
	if (m_Parent)
	{
		m_LocalToWorldMatrix = m_Parent->_ComputeLocalToWorldMatrix();
	}
	else
	{
		m_LocalToWorldMatrix = GetMatrix();
	}
	m_WorldToLocalMatrixIsDitry = false;
	return m_LocalToWorldMatrix;
}

mat4 Transform::_ComputeWorldToLocalMatrix()
{
	m_WorldToLocalMatrix = math::inverse(GetLocalToWorldMatrix());
	return m_WorldToLocalMatrix;
}

void Transform::_UpdateChildrenLocalOrientation()
{
	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		Transform* child = (*it);

		child->SetLocalOrientation(math::gtc::quaternion::quat_cast(GetWorldToLocalMatrix() * math::gtc::quaternion::mat4_cast(GetOrientation())));
	}
}

void Transform::_UpdateChildrenOrientation()
{
	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		Transform* child = (*it);

		child->SetOrientation(math::gtc::quaternion::quat_cast(GetLocalToWorldMatrix() * math::gtc::quaternion::mat4_cast(GetLocalOrientation())));
	}
}

void Transform::_UpdateChildrenLocalPosition()
{
	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		Transform* child = (*it);

		child->SetLocalPosition(vec3(GetWorldToLocalMatrix() * vec4(child->GetPosition(), 1.0f)));
	}
}

void Transform::_UpdateChildrenPosition()
{
	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		Transform* child = (*it);
		
		child->SetPosition(vec3(GetLocalToWorldMatrix() * vec4(child->GetLocalPosition(), 1.0f)));
	}
}

void Transform::_AddChild( Transform* _Child )
{
	assert(_Child != this);
	m_Children.push_back(_Child);
}

void Transform::_RemoveChild( Transform* _Child )
{
	m_Children.remove(_Child);
}

}