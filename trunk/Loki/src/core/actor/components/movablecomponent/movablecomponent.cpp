#include "core/actor/components/movablecomponent/movablecomponent.h"

namespace loki
{

LkMovableComponent::LkMovableComponent()	:
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),	// Identity quaternion. w = 1, x = 0, y = 0, z = 0.
	m_Transformation(glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f)),
	m_MatrixIsDirty(false)
{

}

LkMovableComponent::~LkMovableComponent()
{

}

void LkMovableComponent::Update()
{

}

const glm::vec3& LkMovableComponent::GetPosition() const
{
	return m_Position;
}

const glm::quat& LkMovableComponent::GetOrientation() const
{
	return m_Orientation;
}

glm::vec3 LkMovableComponent::GetOrientationVector() const
{
	return m_Orientation * FORWARD;
}

float LkMovableComponent::GetPitch() const
{
	return glm::gtx::quaternion::pitch(m_Orientation);
}

float LkMovableComponent::GetYaw() const
{
	return glm::gtx::quaternion::yaw(m_Orientation);
}

float LkMovableComponent::GetRoll() const
{
	return glm::gtx::quaternion::roll(m_Orientation);
}

const glm::vec3 LkMovableComponent::GetEulerAngles() const
{
	return glm::gtx::quaternion::eulerAngles(m_Orientation);
}

const glm::mat4x4& LkMovableComponent::GetTransformation()
{
	if (m_MatrixIsDirty)
	{
		_CalculateTransformationMatrix();
		m_MatrixIsDirty = false;
	}
	return m_Transformation;
}

void LkMovableComponent::SetTransformation( const glm::mat4x4& _Matrix )
{
	m_Transformation = _Matrix;
	m_MatrixIsDirty = false;
}

void LkMovableComponent::_CalculateTransformationMatrix()
{
	m_Transformation = glm::translate(glm::gtx::quaternion::toMat4(m_Orientation), m_Position * m_Orientation);
}

void LkMovableComponent::SetPosition( const glm::vec3& _Position )
{
	m_Position = _Position;
	m_MatrixIsDirty = true;

	// TODO: If affected by physics, simply setting a position probably is not the right way to go.
}

void LkMovableComponent::SetOrientation( const glm::quat& _Rotation )
{
	m_Orientation = _Rotation;
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateLocal( const glm::vec3& _Axis, const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateLocalX( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateLocalY( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateLocalZ( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ * m_Orientation);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::Rotate( const glm::vec3& _Axis, const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, _Axis);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateX( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalX);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateY( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalY);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::RotateZ( const float _Angle )
{
	m_Orientation = glm::gtc::quaternion::rotate(m_Orientation, _Angle, math::GlobalZ);
	m_MatrixIsDirty = true;
}

void LkMovableComponent::TranslateLocal( const glm::vec3& _Translation )
{
	m_Position += m_Orientation * _Translation;
	m_MatrixIsDirty = true;
}

void LkMovableComponent::Translate( const glm::vec3& _Translation )
{
	m_Position += _Translation;
	m_MatrixIsDirty = true;
}

void LkMovableComponent::_Init()
{

}

}