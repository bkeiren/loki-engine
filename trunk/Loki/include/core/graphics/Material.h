#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

namespace loki
{

namespace graphics
{

class Texture;
class LkEffect;

class Material
{
public:
	enum ETextureType
	{
		TT_DIFFUSE = 0,
		TT_NORMAL,
		TT_SPECULAR,
		TT_CUSTOM_0,
		TT_CUSTOM_1,
		TT_CUSTOM_2,
		TT_CUSTOM_3,

		_TT_COUNT	// Do not touch.
	};

	const Texture* GetTexture( uint32 _Index ) const;
	void SetTexture( uint32 _Index, Texture* _Texture );

	void SetEffect( LkEffect* _Effect );
private:
	Texture* m_Textures[_TT_COUNT];
	LkEffect* m_Effect;
};

}

}

#endif