#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

namespace loki
{

namespace renderer
{
class LkEffect;
}

namespace graphics
{

class Texture2D;

class Material
{
	CONTAINER_MACRO_HASH_MAP(std::string, Material*, Materials)
public:
	enum ETextureType
	{
		TT_DIFFUSE = 0,
		TT_NORMAL,
		TT_SPECULAR,
		TT_EMISSIVE,
		TT_CUSTOM_0,
		TT_CUSTOM_1,
		TT_CUSTOM_2,
		TT_CUSTOM_3,

		_TT_COUNT	// Do not touch.
	};

	static Material* LoadMaterial( const std::string& _MaterialFile );

	const Texture2D* GetTexture( uint32 _Index ) const;
	void SetTexture( uint32 _Index, Texture2D* _Texture, bool _DeleteOldTexture = true );

	renderer::LkEffect* GetEffect() const;
	void SetEffect( renderer::LkEffect* _Effect );

	f32 GetShininess() const;
	void SetShininess( f32 _Shininess );

	f32 GetReflectivity() const;
	void SetReflectivity( f32 _Reflectivity );

	const vec2& GetUVScale() const;
	void SetUVScale( const vec2& _Scale );

	bool HasDiffuse() const;
	bool HasNormal() const;
	bool HasSpecular() const;
	bool HasEmissive() const;
	bool HasEffect() const;
private:
	Material();
	~Material();

	static Material* _CreateMaterialFromLMAFile( const std::string& _MaterialFile );
	static Material* _FindMaterial( const std::string& _MaterialFile );

	Texture2D* m_Textures[_TT_COUNT];
	renderer::LkEffect* m_Effect;
	static renderer::LkEffect* m_DefaultEffect;
	f32 m_Shininess;
	f32 m_Reflectivity;
	vec2 m_UVScale;

	static Materials m_Materials;
};

}

}

#endif