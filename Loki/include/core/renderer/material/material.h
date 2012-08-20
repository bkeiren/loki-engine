#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

namespace loki
{

namespace renderer
{

class LkTexture;
class LkEffect;

class LkMaterial
{
public:
	LkMaterial( const char* _EffectName, const char* _Diffuse = 0, const char* _Normal = 0, const char* _Specular = 0, const char* _Emissive = 0 );
	~LkMaterial();

	const LkTexture* const GetDiffuse() const;
	const LkTexture* const GetNormal() const;
	const LkTexture* const GetSpecular() const;
	const LkTexture* const GetEmissive() const;
	LkEffect* const GetEffect() const;
	float GetShininess() const;

	bool HasDiffuse() const;
	bool HasNormal() const;
	bool HasSpecular() const;
	bool HasEmissive() const;
	bool HasEffect() const;
	
	void SetShininess( float _Shininess );
private:
	LkTexture* m_DiffuseMap;
	LkTexture* m_NormalMap;
	LkTexture* m_SpecularMap;
	LkTexture* m_EmissiveMap;
	LkEffect* m_Effect;
	float m_Shininess;
};

}

}

#endif