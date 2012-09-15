#include "core/actor/light/directional/directionallight.h"

using namespace loki;

LkDirectionalLight::LkDirectionalLight( const char* _Name, game::LkLevel* _Level )	:
	LkLight(_Name, _Level, LIGHT_DIRECTIONAL),
	m_Direction(vec3(0.0f, -1.0f, 0.0f))
{

}

LkDirectionalLight::LkDirectionalLight()
{

}

LkDirectionalLight::~LkDirectionalLight()
{

}

const vec3& LkDirectionalLight::GetDirection() const
{
	return m_Direction;
}

void LkDirectionalLight::SetDirection( const vec3& _Direction )
{
	m_Direction = _Direction;
}