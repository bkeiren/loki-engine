#include "core/physics/rigidbodyinfo.h"

namespace loki
{

namespace physics
{

RigidBodyInfo::RigidBodyInfo()	:
	m_Shape(CS_BOX),
	m_Mass(1.0f),
	m_Friction(0.5f),
	m_Restitution(0.0f),
	m_InitialTransform(glm::mat4()),
	m_CenterOfMassOffset(glm::mat4()),
	m_LocalInertia(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_LinearDamping(0.0f),
	m_AngularDamping(0.0f),
	m_LinearSleepingThreshold(0.8f),
	m_AngularSleepingThreshold(1.0f),
	m_AdditionalDamping(false),
	m_AdditionalDampingFactor(0.005f),
	m_AdditionalLinearDampingThresholdSqr(0.01f),
	m_AdditionalAngularDampingThresholdSqr(0.01f),
	m_AdditionalAngularDampingFactor(0.01f)
{
	m_CapsuleData.m_Axis = SA_Y;
	m_CapsuleData.m_Radius = 1.0f;
	m_CapsuleData.m_Height = 2.0f;

	m_BoxData.m_HalfExtents = glm::vec3(1.0f, 1.0f, 1.0f);

	m_SphereData.m_Radius = 1.0f;

	m_MeshData.m_Mesh = NULL;

	m_StaticPlaneData.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_StaticPlaneData.m_Constant = 0.0f;

	m_ConeData.m_Axis = SA_Y;
	m_ConeData.m_Radius = 1.0f;
	m_ConeData.m_Height = 1.0f;

	m_CylinderData.m_HalfExtents = glm::vec3(1.0f, 1.0f, 1.0f);
}

}

}