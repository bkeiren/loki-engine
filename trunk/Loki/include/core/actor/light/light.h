#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "core/actor/actor.h"
#include "util/color/color.h"
#include "core/actor/components/movablecomponent/movablecomponent.h"

namespace loki
{

class LkPointLight;
class LkSpotLight;
class LkDirectionalLight;

enum ELightType
{
	LIGHT_POINT = 0,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL
};

class LkLight	: public LkActor
{
	friend class LkPointLight;
	friend class LkSpotLight;
	friend class LkDirectionalLight;
public:

	//////////////////////////////////////////////////////////////////////////
	// Returns the light's type.
	//////////////////////////////////////////////////////////////////////////
	ELightType GetLightType();

	//////////////////////////////////////////////////////////////////////////
	// Returns whether the light is a shadow caster.
	//////////////////////////////////////////////////////////////////////////
	bool IsShadowCaster();

	//////////////////////////////////////////////////////////////////////////
	// Returns the light's color.
	//////////////////////////////////////////////////////////////////////////
	const Color& GetColor() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the light's specular color.
	//////////////////////////////////////////////////////////////////////////
	const Color& GetSpecular() const;

	f32 GetConstantAttenuation() const;

	f32 GetLinearAttenuation() const;

	f32 GetQuadraticAttenuation() const;

	//////////////////////////////////////////////////////////////////////////
	// Sets the light's color.
	//////////////////////////////////////////////////////////////////////////
	void SetColor( const Color& _Color );

	//////////////////////////////////////////////////////////////////////////
	// Sets the light's specular color.
	//////////////////////////////////////////////////////////////////////////
	void SetSpecular( const Color& _Specular );

	void SetConstantAttenuation( f32 _Attenuation );

	void SetLinearAttenuation( f32 _Attenuation );
	
	void SetQuadraticAttenuation( f32 _Attenuation );

	virtual std::string ToString() const;

	void SetEnabled( bool _Enabled );
	void Enable();
	void Disable();
	bool IsEnabled() const;
private:
	LkLight( const char* _Name, game::LkLevel* _Level, ELightType _LightType );
	virtual ~LkLight() = 0;	// '= 0' to make the class abstract.
	LkLight();	// Private default c-tor.

	ELightType m_LightType;
	Color m_LightColor;
	Color m_LightSpecular;
	bool m_CastShadows;
	bool m_Enabled;
	f32 m_ConstantAttenuation;
	f32 m_LinearAttenuation;
	f32 m_QuadraticAttenuation;
};

}	// namespace loki.

#endif	// LIGHT_H