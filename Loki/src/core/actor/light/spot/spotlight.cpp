#include "core/actor/light/spot/spotlight.h"

using namespace loki;

LkSpotLight::LkSpotLight( const char* _Name, game::LkLevel* _Level )	:
	LkLight(_Name, _Level, LIGHT_SPOT),
	m_Direction(glm::vec3(0.0f, -1.0f, 0.0f)),
	m_InnerAngle(30.0f),
	m_OuterAngle(35.0f)
{

}

LkSpotLight::LkSpotLight()
{

}

LkSpotLight::~LkSpotLight()
{

}

const glm::vec3& LkSpotLight::GetDirection() const
{
	return m_Direction;
}

float LkSpotLight::GetInnerAngle() const
{
	return m_InnerAngle;
}

float LkSpotLight::GetOuterAngle() const
{
	return m_OuterAngle;
}