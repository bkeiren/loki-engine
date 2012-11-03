#include "core/entitysystem/component/default/Light.h"
#include "core/graphics/Texture.h"

namespace loki
{

namespace components
{

Light::Lights Light::m_Lights;
Light::Lights Light::m_LightsByType[];
graphics::Texture* Light::m_AttenuationTexture = 0;

Light::Light()	:
	m_LightType(LIGHT_POINT)
	,m_Range(10.0f)
	,m_SpotAngle(25.0f)
	,m_CookieSize(1.0f)
	,m_AreaSize(vec2(1.0f, 1.0f))
	,m_Color(ColorRGB(1.0f, 1.0f, 1.0f))
	,m_Intensity(1.0f)
	,m_ShadowType(SHADOWS_NONE)
	,m_Cookie(0)
{
	if (!m_AttenuationTexture)
	{
		m_AttenuationTexture = graphics::Texture::Load("resources//textures//LightAttenuation.bmp");
		if (!m_AttenuationTexture)
		{
			LOG(VL_ERROR, "Light::Light: Failed to load light attenuation texture");
		}
		else
		{
			m_AttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
			m_AttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_T, graphics::CLAMP_TO_EDGE);
		}
	}
}

Light::~Light()
{

}

Light::Lights& Light::GetAllLights()
{
	return m_Lights;
}

Light::Lights& Light::GetAllLightsByType( ELightType _Type )
{
	return m_LightsByType[_Type];
}

graphics::Texture* Light::GetAttenuationTexture()
{
	return m_AttenuationTexture;
}

Light::ELightType Light::GetLightType() const
{
	return m_LightType;
}

void Light::SetLightType( ELightType _Type )
{
	if (_Type == m_LightType)
	{
		return;
	}

	m_LightsByType[m_LightType].remove(this);
	m_LightType = _Type;
	m_LightsByType[m_LightType].push_back(this);
}

f32 Light::GetRange() const
{
	return m_Range;
}

void Light::SetRange( f32 _Range )
{
	m_Range = _Range;
}

const ColorRGB& Light::GetColor() const
{
	return m_Color;
}

void Light::SetColor( const ColorRGB& _Color )
{
	m_Color = _Color;
}

f32 Light::GetIntensity() const
{
	return m_Intensity;
}

void Light::SetIntensity( f32 _Intensity )
{
	m_Intensity = math::clamp(_Intensity, 0.0f, 8.0f);
}

Light::EShadowType Light::GetShadowType() const
{
	return m_ShadowType;
}

void Light::SetShadowType( EShadowType _ShadowType )
{
	m_ShadowType = _ShadowType;
}

void Light::_OnEvent( const LkEvent& _Event )
{
	
}

void Light::_Init()
{
	m_Lights.push_back(this);
}

void Light::_Terminate()
{
	m_Lights.remove(this);
	m_LightsByType[GetLightType()].remove(this);
}

}

}