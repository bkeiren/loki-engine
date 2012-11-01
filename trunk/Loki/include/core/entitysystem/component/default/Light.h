#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "core/entitysystem/component/Component.h"
#include "util/color/color.h"

namespace loki
{

namespace graphics
{
	class Texture;
}

namespace components
{

class Light	: public Component
{
public:
	DECLARE_COMPONENT_TYPEINFO(Light)

	CONTAINER_MACRO_LIST(Light*, Lights);

	enum ELightType
	{
		LIGHT_POINT = 0
		,LIGHT_SPOT
		,LIGHT_DIRECTIONAL
		//,LIGHT_AREA	// To be implemented.

		,_LIGHT_COUNT
	};

	enum EShadowType
	{
		SHADOWS_NONE = 0
		,SHADOWS_HARD
		,SHADOWS_SOFT

		,_SHADOWS_COUNT
	};

	Light();
	~Light();

	static Lights& GetAllLights();
	static Lights& GetAllLightsByType( ELightType _Type );

	ELightType GetLightType() const;
	void SetLightType( ELightType _Type );

	f32 GetRange() const;
	void SetRange( f32 _Range );

	const ColorRGB& GetColor() const;
	void SetColor( const ColorRGB& _Color );

	f32 GetIntensity() const;
	void SetIntensity( f32 _Intensity );

	EShadowType GetShadowType() const;
	void SetShadowType( EShadowType _ShadowType );
private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();

	ELightType m_LightType;

	// For point and spot lights.
	f32 m_Range;

	// For spot lights.
	f32 m_SpotAngle;

	// For directional lights.
	f32 m_CookieSize;

	// For area lights.
	vec2 m_AreaSize;	// Only X and Y components because area lights are actually planes, not boxes.

	// For all light types.
	ColorRGB m_Color;
	f32 m_Intensity;	// Default 1.0. Min 0.0, max 8.0.
	EShadowType m_ShadowType;
	graphics::Texture* m_Cookie;	// 2D for spot and directional lights, cubemap for point lights.


	// All lights in one convenient list.
	static Lights m_Lights;

	// All lights of each type in their own list.
	static Lights m_LightsByType[_LIGHT_COUNT];
};

}

}

#endif