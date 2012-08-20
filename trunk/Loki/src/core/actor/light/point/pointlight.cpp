#include "core/actor/light/point/pointlight.h"

using namespace loki;

LkPointLight::LkPointLight( const char* _Name, game::LkLevel* _Level )	:
	LkLight(_Name, _Level, LIGHT_POINT),
	m_Radius(5.0f),
	m_FalloffExponent(2.0f)
{

}

LkPointLight::LkPointLight()
{

}

LkPointLight::~LkPointLight()
{

}

float LkPointLight::GetRadius() const
{
	return m_Radius;
}

float LkPointLight::GetFalloffExponent() const
{
	return m_FalloffExponent;
}

void LkPointLight::SetRadius( float _Radius )
{
	m_Radius = _Radius;
}

void LkPointLight::SetFalloffExponent( float _FalloffExponent )
{
	m_FalloffExponent = _FalloffExponent;
}