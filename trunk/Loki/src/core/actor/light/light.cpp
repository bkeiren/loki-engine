#include <sstream>
#include "core/actor/light/light.h"

using namespace loki;

LkLight::LkLight( const char* _Name, game::LkLevel* _Level, ELightType _LightType )	:
	LkActor(_Name, _Level),
	m_LightType(_LightType),
	m_LightColor(Color(1.0f, 1.0f, 1.0f)),
	m_LightSpecular(Color(1.0f, 1.0f, 1.0f)),
	m_CastShadows(false),
	m_Enabled(true),
	m_ConstantAttenuation(0.0f),
	m_LinearAttenuation(1.0f),
	m_QuadraticAttenuation(0.5f)
{
	// Lights need a movable component.
	AddComponent<LkMovableComponent>();
}

LkLight::LkLight()
{

}

LkLight::~LkLight()
{

}

ELightType LkLight::GetLightType()
{
	return m_LightType;
}

bool LkLight::IsShadowCaster()
{
	return m_CastShadows;
}

const Color& LkLight::GetColor() const
{
	return m_LightColor;
}

const Color& LkLight::GetSpecular() const
{
	return m_LightSpecular;
}

f32 LkLight::GetConstantAttenuation() const
{
	return m_ConstantAttenuation;
}

f32 LkLight::GetLinearAttenuation() const
{
	return m_LinearAttenuation;
}

f32 LkLight::GetQuadraticAttenuation() const
{
	return m_QuadraticAttenuation;
}

void LkLight::SetColor( const Color& _Color )
{
	m_LightColor = _Color;
}

void LkLight::SetSpecular( const Color& _Specular )
{
	m_LightSpecular = _Specular;
}

void LkLight::SetConstantAttenuation( f32 _Attenuation )
{
	m_ConstantAttenuation = _Attenuation;
}

void LkLight::SetLinearAttenuation( f32 _Attenuation )
{
	m_LinearAttenuation = _Attenuation;
}

void LkLight::SetQuadraticAttenuation( f32 _Attenuation )
{
	m_QuadraticAttenuation = _Attenuation;
}

std::string LkLight::ToString() const
{
	std::string LightTypeStr;
	
	switch(m_LightType)
	{
	case LIGHT_POINT:
		LightTypeStr = "LIGHT_POINT";
		break;
	case LIGHT_SPOT:
		LightTypeStr = "LIGHT_DIRECTIONAL";
		break;
	case LIGHT_DIRECTIONAL:
		LightTypeStr = "LIGHT_DIRECTIONAL";
		break;
	}

	std::stringstream str;
	str << LkActor::ToString() <<		\
		   "\nLightType:\t" << LightTypeStr <<	\
		   "\nLightColor:\t[ " << m_LightColor.r << ", " << m_LightColor.g << ", " << m_LightColor.b << " ]" <<	\
		   "\nCastShadow:\t" << (m_CastShadows)?("yes"):("no");

	return str.str();
}

void LkLight::SetEnabled( bool _Enabled )
{
	m_Enabled = _Enabled;
}

void LkLight::Enable()
{
	m_Enabled = true;
}

void LkLight::Disable()
{
	m_Enabled = false;
}

bool LkLight::IsEnabled() const
{
	return m_Enabled;
}