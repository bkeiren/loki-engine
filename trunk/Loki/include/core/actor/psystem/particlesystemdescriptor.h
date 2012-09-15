#pragma once

#ifndef PARTICLESYSTEMDESCRIPTOR_H
#define PARTICLESYSTEMDESCRIPTOR_H

#include "core/actor/psystem/particlesourcedescriptor.h"
#include <vector>

namespace loki
{

class LkParticleSystemDescriptor
{
	typedef std::vector<LkParticleSourceDescriptor>		ParticleSources;
	typedef ParticleSources::iterator					ParticleSourcesIter;
	typedef ParticleSources::const_iterator				ParticleSourcesConstIter;
	typedef ParticleSources::reverse_iterator			ParticleSourcesRIter;
	typedef ParticleSources::const_reverse_iterator		ParticleSourcesConstRIter;

	friend class LkParticleSystem;
public:
	LkParticleSystemDescriptor();
	~LkParticleSystemDescriptor();

	LkParticleSourceDescriptor& AddSource();

	vec3 m_Position;	// Particle system origin.
private:
	ParticleSources m_SourceDescriptors;
};

}

#endif