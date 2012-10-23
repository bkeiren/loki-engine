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

f32 LkPointLight::GetRadius() const
{
	return m_Radius;
}

f32 LkPointLight::GetFalloffExponent() const
{
	return m_FalloffExponent;
}

void LkPointLight::SetRadius( f32 _Radius )
{
	m_Radius = _Radius;
}

void LkPointLight::SetFalloffExponent( f32 _FalloffExponent )
{
	m_FalloffExponent = _FalloffExponent;
}