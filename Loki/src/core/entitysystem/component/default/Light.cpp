#include "core/entitysystem/component/default/Light.h"
#include "core/graphics/Texture.h"

namespace loki
{

namespace components
{

Light::Lights Light::m_Lights;
Light::Lights Light::m_LightsByType[];
graphics::Texture* Light::m_PointAttenuationTexture = 0;
graphics::Texture* Light::m_SpotAttenuationTexture = 0;

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
	SetSpotAngle(30.0f);

	if (!m_PointAttenuationTexture)
	{
		m_PointAttenuationTexture = graphics::Texture::Load("resources//textures//PointLightAttenuation.bmp");
		if (!m_PointAttenuationTexture)
		{
			LOG(VL_ERROR, "Light::Light: Failed to load pointlight attenuation texture");
		}
		else
		{
			m_PointAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
			m_PointAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_T, graphics::CLAMP_TO_EDGE);
		}
	}
	if (!m_SpotAttenuationTexture)
	{
		m_SpotAttenuationTexture = graphics::Texture::Load("resources//textures//SpotLightAttenuation.bmp");
		if (!m_SpotAttenuationTexture)
		{
			LOG(VL_ERROR, "Light::Light: Failed to load spotlight attenuation texture");
		}
		else
		{
			m_SpotAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
			m_SpotAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_T, graphics::CLAMP_TO_EDGE);
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

graphics::Texture* Light::GetPointAttenuationTexture()
{
	return m_PointAttenuationTexture;
}

graphics::Texture* Light::GetSpotAttenuationTexture()
{
	return m_SpotAttenuationTexture;
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

f32 Light::GetSpotAngle() const
{
	return m_SpotAngle;
}

void Light::SetSpotAngle( f32 _Angle )
{
	m_SpotAngle = math::clamp(_Angle, 1.0f, 179.0f);
	m_SpotBaseTangent = math::tan(math::radians(m_SpotAngle * 0.5f));
}

f32 Light::GetSpotBaseRadius() const
{
	return GetRange() * m_SpotBaseTangent;
}

void Light::_HandleEvent( const LkEvent& _Event )
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