#include "core/actor/components/moveablecomponent/moveablecomponent.h"

namespace loki
{

LkMoveableComponent::LkMoveableComponent()	:
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),	// Identity quaternion. w = 1, x = 0, y = 0, z = 0.
	m_Transformation(glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f)),
	m_MatrixIsDirty(false)
{

}

LkMoveableComponent::~LkMoveableComponent()
{

}

void LkMoveableComponent::Update()
{

}

const glm::vec3& LkMoveableComponent::GetPosition() const
{
	return m_Position;
}

const glm::quat& LkMoveableComponent::GetOrientation() const
{
	return m_Orientation;
}

glm::vec3 LkMoveableComponent::GetOrientationVector() const
{
	return m_Orientation * FORWARD;
}

float LkMoveableComponent::GetPitch() const
{
	return glm::gtx::quaternion::pitch(m_Orientation);
}

float LkMoveableComponent::GetYaw() const
{
	return glm::gtx::quaternion::yaw(m_Orientation);
}

float LkMoveableComponent::GetRoll() const
{
	return glm::gtx::quaternion::roll(m_Orientation);
}

const glm::vec3 LkMoveableComponent::GetEulerAngles() const
{
	return glm::gtx::quaternion::eulerAngles(m_Orientation);
}

const glm::mat4x4& LkMoveableComponent::GetTransformation()
{
	if (m_MatrixIsDirty)
	{
		_CalculateTransformationMatrix();
		m_MatrixIsDirty = false;
	}
	return m_Transformation;
}

void LkMoveableComponent::SetTransformation( const glm::mat4x4& _Matrix )
{
	m_Transformation = _Matrix;
	m_MatrixIsDirty = false;
}

void LkMoveableComponent::_CalculateTransformationMatrix()
{
	m_Transformation = glm::translate(glm::gtx::quaternion::toMat4(m_Orientation), m_Position * m_Orientation);
}

void LkMoveableComponent::SetPosition( const glm::vec3& _Position )
{
	m_Position = _Position;
	m_MatrixIsDirty = true;

	// TODO: If affected by physics, simply setting a position probably is not the right way to go.
}

void LkMoveableComponent::SetOrientation( const glm::quat& _Rotation )
{
	m_Orientation = _Rotation;
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateLocal( const glm::vec3& _Axis, const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateLocalX( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateLocalY( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateLocalZ( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::Rotate( const glm::vec3& _Axis, const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateX( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateY( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::RotateZ( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ);
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::TranslateLocal( const glm::vec3& _Translation )
{
	m_Position += m_Orientation * _Translation;
	m_MatrixIsDirty = true;
}

void LkMoveableComponent::Translate( const glm::vec3& _Translation )
{
	m_Position += _Translation;
	m_MatrixIsDirty = true;
}

}