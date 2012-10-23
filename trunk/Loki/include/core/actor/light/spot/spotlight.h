#pragma once

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "core/actor/light/light.h"

namespace loki
{

namespace game
{

class LkLevel;

}

class LkSpotLight	: public LkLight
{
	friend class game::LkLevel;
public:
	//////////////////////////////////////////////////////////////////////////
	// Returns the spotlight's direction.
	//////////////////////////////////////////////////////////////////////////
	const vec3& GetDirection() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the inner cone angle.
	//////////////////////////////////////////////////////////////////////////
	f32 GetInnerAngle() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the outer cone angle.
	//////////////////////////////////////////////////////////////////////////
	f32 GetOuterAngle() const;
private:
	LkSpotLight( const char* _Name, game::LkLevel* _Level );
	LkSpotLight();	// Private default c-tor.
	~LkSpotLight();

	vec3 m_Direction;
	f32 m_InnerAngle;
	f32 m_OuterAngle;
};

}

#endif // SPOTLIGHT_H