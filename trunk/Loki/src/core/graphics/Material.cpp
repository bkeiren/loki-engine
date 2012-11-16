#include "core/graphics/Material.h"
#include "core/graphics/Texture2D.h"
#include "core/renderer/effect/effectmanager.h"

namespace loki
{

namespace graphics
{

renderer::LkEffect* Material::m_DefaultEffect = 0;

Material::Material()	:
	m_Effect(0),
	m_Shininess(50.0f),
	m_Reflectivity(0.0f)
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		m_Textures[i] = 0;
	}

	if (!m_DefaultEffect)
	{
		m_DefaultEffect = renderer::g_EffectManager->CreateEffectFromFile(DEFAULT_RESOURCE("shaders//gbuffer_default.cgfx"), "DefaultMaterialEffect");
	}
}

Material::~Material()
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		delete m_Textures[i];
	}
}

const Texture2D* Material::GetTexture( uint32 _Index ) const
{
	return m_Textures[_Index];
}

void Material::SetTexture( uint32 _Index, Texture2D* _Texture, bool _DeleteOldTexture /*= true*/ )
{
	if (_DeleteOldTexture)
	{
		delete m_Textures[_Index];
	}
	m_Textures[_Index] = _Texture;
}

renderer::LkEffect* Material::GetEffect() const
{
	return m_Effect == 0 ? m_DefaultEffect : m_Effect;
}

void Material::SetEffect( renderer::LkEffect* _Effect )
{
	m_Effect = _Effect;
}

f32 Material::GetShininess() const
{
	return m_Shininess;
}

void Material::SetShininess( f32 _Shininess )
{
	m_Shininess = _Shininess;
}

f32 Material::GetReflectivity() const
{
	return m_Reflectivity;
}

void Material::SetReflectivity( f32 _Reflectivity )
{
	m_Reflectivity = math::clamp(_Reflectivity, 0.0f, 1.0f);
}

bool Material::HasDiffuse() const
{
	return m_Textures[TT_DIFFUSE] != 0;
}

bool Material::HasNormal() const
{
	return m_Textures[TT_NORMAL] != 0;
}

bool Material::HasSpecular() const
{
	return m_Textures[TT_SPECULAR] != 0;
}

bool Material::HasEmissive() const
{
	return m_Textures[TT_EMISSIVE] != 0;
}

bool Material::HasEffect() const
{
	return m_Effect != 0;
}

}

}