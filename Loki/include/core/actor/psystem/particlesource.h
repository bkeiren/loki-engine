#pragma once

#ifndef PARTICLESOURCE_H
#define PARTICLESOURCE_H

#include "core/actor/components/movablecomponent/movablecomponent.h"
#include "core/actor/actor.h"
#include <vector>

namespace loki
{

class LkParticle;

class LkParticleSource	: public LkMovableComponent
{
	typedef std::vector<LkParticle*>			Particles;
	typedef Particles::iterator					ParticlesIter;
	typedef Particles::const_iterator			ParticlesConstIter;
	typedef Particles::reverse_iterator			ParticlesRIter;
	typedef Particles::const_reverse_iterator	ParticlesConstRIter;

	friend class LkParticleSystem;
public:
	void SetQuota( int _Quota );
	int GetQuota() const;

private:
	LkParticleSource( int _Quota = 1 );
	~LkParticleSource();

	void _Update();

	//////////////////////////////////////////////////////////////////////////
	// Spawns a number of particles (if possible) from the available pool.
	//////////////////////////////////////////////////////////////////////////
	void _Spawn( int _Quota );

	Particles m_Particles;
	float m_SpawnRate;	// Particle spawns per second.
	float m_Age;
	float m_Lifetime;
	float m_Birth;	// Moment of birth after it's parent system spawns. This makes it possible to build some sort of delay
					// before the particle source springs into action.
	float m_AgeSinceLastSpawn;
	int m_SpawnQuota;	// Number of particles to spawn per spawning-moment.
};

}

#endif