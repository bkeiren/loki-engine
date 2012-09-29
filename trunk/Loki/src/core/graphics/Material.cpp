#include "core/graphics/Material.h"

namespace loki
{

namespace graphics
{

Material::Material()	:
	m_Effect(0),
	m_Shininess(1.0f)
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

float Material::GetShininess() const
{
	return m_Shininess;
}

void Material::SetShininess( float _Shininess )
{
	m_Shininess = _Shininess;
}

}

}