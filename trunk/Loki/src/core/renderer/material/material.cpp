#include "core/renderer/material/material.h"
#include "core/resourcemanager/texturemanager.h"
#include "core/renderer/effect/effectmanager.h"

namespace loki
{

namespace renderer
{

LkMaterial::LkMaterial( const char* _EffectName, const char* _Diffuse /*= 0*/, const char* _Normal /*= 0*/, const char* _Specular /*= 0*/, const char* _Emissive /*= 0*/ )	:
	m_DiffuseMap(0),
	m_NormalMap(0),
	m_SpecularMap(0),
	m_EmissiveMap(0),
	m_Effect(0),
	m_Shininess(50.0f)
{
	if (_Diffuse)	m_DiffuseMap = g_TextureManager->GetResource(_Diffuse);
	if (_Normal)	m_NormalMap = g_TextureManager->GetResource(_Normal);
	if (_Specular)	m_SpecularMap = g_TextureManager->GetResource(_Specular);
	if (_Emissive)	m_EmissiveMap = g_TextureManager->GetResource(_Emissive);
	
	if (_EffectName)	m_Effect = g_EffectManager->GetEffect(_EffectName);	// Might return 0. Effect manager will output error.
}

LkMaterial::~LkMaterial()
{
#define RELEASE_TEX(tex)	{if (tex != 0) g_TextureManager->ReleaseResource(&tex);}

	RELEASE_TEX(m_DiffuseMap)
	RELEASE_TEX(m_NormalMap)
	RELEASE_TEX(m_SpecularMap)
	RELEASE_TEX(m_EmissiveMap)

#undef RELEASE_TEX
}

const LkTexture* const LkMaterial::GetDiffuse() const
{
	return m_DiffuseMap;
}

const LkTexture* const LkMaterial::GetNormal() const
{
	return m_NormalMap;
}

const LkTexture* const LkMaterial::GetSpecular() const
{
	return m_SpecularMap;
}

const LkTexture* const LkMaterial::GetEmissive() const
{
	return m_EmissiveMap;
}

LkEffect* const LkMaterial::GetEffect() const
{
	return m_Effect;
}

float LkMaterial::GetShininess() const
{
	return m_Shininess;
}

bool LkMaterial::HasDiffuse() const
{
	return (m_DiffuseMap != 0);
}

bool LkMaterial::HasNormal() const
{
	return (m_NormalMap != 0);
}

bool LkMaterial::HasSpecular() const
{
	return (m_SpecularMap != 0);
}

bool LkMaterial::HasEmissive() const
{
	return (m_EmissiveMap != 0);
}

bool LkMaterial::HasEffect() const
{
	return (m_Effect != 0);
}

void LkMaterial::SetShininess( float _Shininess )
{
	m_Shininess = _Shininess;
}

}

}