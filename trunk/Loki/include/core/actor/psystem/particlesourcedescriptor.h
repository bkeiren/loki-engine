#pragma once

#ifndef PARTICLESOURCEDESCRIPTOR_H
#define PARTICLESOURCEDESCRIPTOR_H

#include "core/actor/psystem/particle.h"

namespace loki
{

class LkParticleSourceDescriptor
{
	friend class LkParticleSystemDescriptor;

	// These next two lines are required in order to be able to have a vector of LkParticleSourceDescriptor's.
	friend class std::vector<LkParticleSourceDescriptor>;
	friend void std::_Destroy<LkParticleSourceDescriptor>( LkParticleSourceDescriptor _FARQ* _Ptr );
public:
	float		m_SpawnRate;	// Particle spawns per second.
	float		m_Lifetime;		// Lifetime duration.
	float		m_Birth;		// Moment of birth after it's parent system spawns. This makes it possible to build some sort of delay
								// before the particle source springs into action.
	int			m_Quota;		// Maximum number of particles that can be alive at any one time.
	int			m_SpawnQuota;	// Number of particles to spawn per spawning-moment.
	void*		m_DefaultUserData[8];	// Default user data.
	glm::vec3	m_Position;		// Source position, relative to system position.
	LkParticle::ParticleCallback m_Callback;	// Particle callback function which is called every frame.
private:
	LkParticleSourceDescriptor();
	~LkParticleSourceDescriptor();
};

}

#endif