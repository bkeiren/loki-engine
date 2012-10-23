#include "core/graphics/Material.h"
#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

Material::Material()	:
	m_Effect(0),
	m_Shininess(50.0f)
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		m_Textures[i] = 0;
	}
}

Material::~Material()
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		delete m_Textures[i];
	}
}

const Texture* Material::GetTexture( uint32 _Index ) const
{
	return m_Textures[_Index];
}

void Material::SetTexture( uint32 _Index, Texture* _Texture, bool _DeleteOldTexture /*= true*/ )
{
	if (_DeleteOldTexture)
	{
		delete m_Textures[_Index];
	}
	m_Textures[_Index] = _Texture;
}

renderer::LkEffect* Material::GetEffect() const
{
	return m_Effect;
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