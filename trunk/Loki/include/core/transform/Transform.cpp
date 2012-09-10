#include "core/transform/Transform.h"

namespace loki
{

Transform::Transform()	:
	m_Orientation(glm::quat()),
	m_Translation(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Transformation(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 
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

const glm::quat& Transform::GetOrientation()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return m_Orientation;
}

const glm::vec3& Transform::GetTranslation()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return m_Translation;
}

const glm::mat4& Transform::GetMatrix()
{
	if (m_MatrixIsDirty)
	{
		_GenerateMatrix();
	}

	return m_Transformation;
}

glm::vec3 Transform::GetOrientationVector()
{
	return m_Orientation * FORWARD;
}

glm::vec3 Transform::GetEulerAngles()
{
	if (m_TranslationOrientationAreDirty)
	{
		_GenerateTranslationOrientation();
	}

	return glm::gtx::quaternion::eulerAngles(m_Orientation);
}

float Transform::GetPitch() const
{
	return glm::gtx::quaternion::pitch(m_Orientation);
}

float Transform::GetYaw() const
{
	return glm::gtx::quaternion::yaw(m_Orientation);
}

float Transform::GetRoll() const
{
	return glm::gtx::quaternion::roll(m_Orientation);
}

void Transform::SetOrientation( const glm::quat& _Orientation )
{
	m_MatrixIsDirty = true;

	m_Orientation = _Orientation;
}

void Transform::SetTranslation( const glm::vec3& _Translation )
{
	m_MatrixIsDirty = true;

	m_Translation = _Translation;
}

void Transform::SetMatrix( const glm::mat4& _Matrix )
{
	m_TranslationOrientationAreDirty = true;

	m_Transformation = _Matrix;
}

void Transform::TranslateLocal( const glm::vec3& _Translation )
{
	m_Translation += m_Orientation * _Translation;
	m_MatrixIsDirty = true;
}

void Transform::TranslateWorld( const glm::vec3& _Translation )
{
	m_Translation += _Translation;
	m_MatrixIsDirty = true;
}

void Transform::RotateXLocal( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateYLocal( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateZLocal( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateLocal( glm::vec3& _Axis, float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis * m_Orientation);
	m_MatrixIsDirty = true;
}

void Transform::RotateXWorld( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX);
	m_MatrixIsDirty = true;
}

void Transform::RotateYWorld( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY);
	m_MatrixIsDirty = true;
}

void Transform::RotateZWorld( float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ);
	m_MatrixIsDirty = true;
}

void Transform::RotateWorld( glm::vec3& _Axis, float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis);
	m_MatrixIsDirty = true;
}

bool Transform::IsDirty() const
{
	return (m_MatrixIsDirty || m_TranslationOrientationAreDirty);
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
	m_Transformation = glm::translate(glm::gtx::quaternion::toMat4(m_Orientation), m_Translation * m_Orientation);
	m_MatrixIsDirty = false;
}

void Transform::_GenerateTranslationOrientation()
{
	m_Orientation = glm::gtc::quaternion::quat_cast(m_Transformation);
	m_Translation = glm::vec3(m_Transformation[3][0], m_Transformation[3][1], m_Transformation[3][2]);
	m_TranslationOrientationAreDirty = false;
}

}