#pragma once

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "core/actor/light/light.h"

namespace loki
{

class LkDirectionalLight	: public LkLight
{
	friend class game::LkLevel;
public:
	//////////////////////////////////////////////////////////////////////////
	// Returns the directional light's direction.
	//////////////////////////////////////////////////////////////////////////
	const vec3& GetDirection() const;

	//////////////////////////////////////////////////////////////////////////
	// Sets the directional light's direction.
	//////////////////////////////////////////////////////////////////////////
	void SetDirection( const vec3& _Direction );
private:
	LkDirectionalLight( const char* _Name, game::LkLevel* _Level );
	LkDirectionalLight();
	~LkDirectionalLight();

	vec3 m_Direction;
};

}

#endif // DIRECTIONALLIGHT_H