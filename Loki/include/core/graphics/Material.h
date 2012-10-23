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

class Texture;

class Material
{
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

	Material();
	~Material();

	const Texture* GetTexture( uint32 _Index ) const;
	void SetTexture( uint32 _Index, Texture* _Texture, bool _DeleteOldTexture = true );

	renderer::LkEffect* GetEffect() const;
	void SetEffect( renderer::LkEffect* _Effect );

	f32 GetShininess() const;
	void SetShininess( f32 _Shininess );

	bool HasDiffuse() const;
	bool HasNormal() const;
	bool HasSpecular() const;
	bool HasEmissive() const;
	bool HasEffect() const;
private:

	Texture* m_Textures[_TT_COUNT];
	renderer::LkEffect* m_Effect;
	f32 m_Shininess;
};

}

}

#endif