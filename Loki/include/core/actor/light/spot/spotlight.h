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
	const glm::vec3& GetDirection() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the inner cone angle.
	//////////////////////////////////////////////////////////////////////////
	float GetInnerAngle() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the outer cone angle.
	//////////////////////////////////////////////////////////////////////////
	float GetOuterAngle() const;
private:
	LkSpotLight( const char* _Name, game::LkLevel* _Level );
	LkSpotLight();	// Private default c-tor.
	~LkSpotLight();

	glm::vec3 m_Direction;
	float m_InnerAngle;
	float m_OuterAngle;
};

}

#endif // SPOTLIGHT_H