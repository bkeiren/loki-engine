#include "core/transform/Transform.h"

namespace loki
{

Transform::Transform()	:
	m_Orientation(quat()),
	m_Translation(vec3(0.0f, 0.0f, 0.0f)),
	m_Transformation(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
							   0.0f, 1.0f, 0.0f, 0.0f, 
							   0.0f, 0.0f, 1.0f, 0.0f, 
							   0.0f, 0.0, 0.0f, 1.0f)),
   m_MatrixIsDirty(false),
   m_TranslationOrientationAreDirty(false)
{

}

Transform::~Transform()
{

}

const quat& Transform::GetOrientation()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return m_Orientation;
}

const vec3& Transform::GetTranslation()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return m_Translation;
}

const mat4& Transform::GetMatrix()
{
	if (m_MatrixIsDirty)
	{
		_GenerateMatrix();
	}

	return m_Transformation;
}

vec3 Transform::GetOrientationVector()
{
	return m_Orientation * FORWARD;
}

vec3 Transform::GetEulerAngles()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return math::gtx::quaternion::eulerAngles(m_Orientation);
}

f32 Transform::GetPitch() const
{
	return math::gtx::quaternion::pitch(m_Orientation);
}

f32 Transform::GetYaw() const
{
	return math::gtx::quaternion::yaw(m_Orientation);
}

f32 Transform::GetRoll() const
{
	return math::gtx::quaternion::roll(m_Orientation);
}

void Transform::SetOrientation( const quat& _Orientation )
{
	m_MatrixIsDirty = true;

	m_Orientation = _Orientation;
}

void Transform::SetTranslation( const vec3& _Translation )
{
	m_MatrixIsDirty = true;

	m_Translation = _Translation;
}

void Transform::SetMatrix( const mat4& _Matrix )
{
	m_TranslationOrientationAreDirty = true;

	m_Transformation = _Matrix;
}

void Transform::TranslateLocal( const vec3& _Translation )
{
	m_Translation += m_Orientation * _Translation;
	m_MatrixIsDirty = true;
}

void Transform::TranslateWorld( const vec3& _Translation )
{
	m_Translation += _Translation;
	m_MatrixIsDirty = true;
}

void Transform::RotateXLocal( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalX * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateYLocal( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalY * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateZLocal( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalZ * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateLocal( vec3& _Axis, f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateXWorld( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalX);
	m_MatrixIsDirty = true;
}

void Transform::RotateYWorld( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalY);
	m_MatrixIsDirty = true;
}

void Transform::RotateZWorld( f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, GlobalZ);
	m_MatrixIsDirty = true;
}

void Transform::RotateWorld( vec3& _Axis, f32 _Angle )
{
	m_Orientation = math::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis);
	m_MatrixIsDirty = true;
}

bool Transform::IsDirty() const
{
	return (m_MatrixIsDirty || m_TranslationOrientationAreDirty);
}

void Transform::LookAt( const vec3& _Target )
{
	mat4 mat = math::gtc::matrix_transform::lookAt(-GetTranslation(), -_Target,	GlobalY);
	SetMatrix(mat);
}

bool Transform::operator == ( Transform& _Transform )
{
	return (GetMatrix() == _Transform.GetMatrix());
}

bool Transform::operator != ( Transform& _Transform )
{
	return (GetMatrix() != _Transform.GetMatrix());
}

void Transform::_GenerateMatrix()
{
	m_Transformation = glm::translate(math::gtx::quaternion::toMat4(m_Orientation), m_Translation * m_Orientation);
	m_MatrixIsDirty = false;
}

void Transform::_GenerateTranslationOrientation()
{
	m_Orientation = math::gtc::quaternion::quat_cast(m_Transformation);
	m_Translation = vec3(m_Transformation[3][0], m_Transformation[3][1], m_Transformation[3][2]);
	m_TranslationOrientationAreDirty = false;
}

}