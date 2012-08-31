#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "core/actor/components/movablecomponent/movablecomponent.h"

namespace loki
{

class LkParticle	: public LkMovableComponent
{
	friend class LkParticleSource;
public:
	float m_Age;
	float m_Lifetime;
	glm::vec4 m_Color;	// Color adjuster (Multiplicative), including alpha.

	bool IsAlive() const;
	void Kill();
	void Resurrect();
	void Reset();
private:
	LkParticle();
	~LkParticle();

	void _Update();

	bool m_IsAlive;
};

}

#endif