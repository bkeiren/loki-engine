#pragma once

#ifndef PARTICLESOURCE_H
#define PARTICLESOURCE_H

#include "core/actor/components/movablecomponent/movablecomponent.h"
#include "core/actor/actor.h"
#include <vector>
#include "core/actor/psystem/particlesourcedescriptor.h"

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
	LkParticleSource( const LkParticleSourceDescriptor& _Descriptor );
	~LkParticleSource();

	void _Update();

	void _Spawn();

	const LkParticleSourceDescriptor& m_Descriptor;
	Particles m_Particles;
	float m_Age;
	float m_AgeSinceLastSpawn;
};

}

#endif