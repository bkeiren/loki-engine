#include "core/entitysystem/component/default/Light.h"
#include "core/graphics/Texture2D.h"
#include "core/graphics/DisplayList.h"
#include "core/graphics/UtilityPrimitives.h"

namespace loki
{

namespace components
{

Light::Lights Light::m_Lights;
Light::Lights Light::m_LightsByType[];
graphics::Texture2D* Light::m_PointAttenuationTexture = 0;
graphics::Texture2D* Light::m_SpotAttenuationTexture = 0;

Light::Light()	:
	m_LightType(LIGHT_POINT)
	,m_Range(10.0f)
	,m_Geometry(0)
	,m_GeometryIsDirty(true)
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
		m_PointAttenuationTexture = graphics::Texture2D::Load("resources//textures//PointLightAttenuation.bmp");
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
		m_SpotAttenuationTexture = graphics::Texture2D::Load("resources//textures//SpotLightAttenuation.bmp");
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

	m_Geometry = graphics::DisplayList::Create(1);
	if (!m_Geometry)
	{
		LOG(VL_ERROR, "Light::Light: Failed to create display list for light geometry");
	}
}

Light::~Light()
{
	delete m_Geometry;
}

Light::Lights& Light::GetAllLights()
{
	return m_Lights;
}

Light::Lights& Light::GetAllLightsByType( ELightType _Type )
{
	return m_LightsByType[_Type];
}

graphics::Texture2D* Light::GetPointAttenuationTexture()
{
	return m_PointAttenuationTexture;
}

graphics::Texture2D* Light::GetSpotAttenuationTexture()
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

	m_GeometryIsDirty = true;
}

f32 Light::GetRange() const
{
	return m_Range;
}

void Light::SetRange( f32 _Range )
{
	m_Range = _Range;

	m_GeometryIsDirty = true;
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
	
	m_GeometryIsDirty = true;
}

f32 Light::GetSpotBaseRadius() const
{
	return GetRange() * m_SpotBaseTangent;
}

graphics::DisplayList* Light::GetGeometry()
{
	// Update light geometry if required.
	if (m_GeometryIsDirty)
	{
		_GenerateGeometry();
		m_GeometryIsDirty = false;
	}
	return m_Geometry;
}

graphics::Texture* Light::GetCookie() const
{
	return m_Cookie;
}

void Light::SetCookie( graphics::Texture* _Texture )
{
	m_Cookie = _Texture;
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

void Light::_GenerateGeometry()
{
	m_Geometry->BeginList();
		
	switch (m_LightType)
	{
	case LIGHT_POINT:
		{
			graphics::DrawIcoSphere(GetRange(), 1);
			break;
		}
	case LIGHT_SPOT:
		{
			graphics::DrawCone(GetSpotBaseRadius(), GetRange(), 20);
			break;
		}
	default:
		{
			static const char* LightTypesStrings[4] = {"LIGHT_POINT", "LIGHT_SPOT", "LIGHT_DIRECTIONAL", "LIGHT_AREA"};
			LOG(VL_WARN, "Light::_GenerateGeometry: No geometry to generate for a light of type %s", LightTypesStrings[m_LightType]);
			break;
		}
	}

	m_Geometry->EndList();
}

}

}