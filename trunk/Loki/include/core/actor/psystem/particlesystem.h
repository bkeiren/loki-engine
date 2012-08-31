#pragma once

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "core/actor/psystem/particlesystemdescriptor.h"
#include "core/actor/actor.h"
#include <vector>

namespace loki
{

namespace game
{
	class LkLevel;
}

class LkParticleSource;

class LkParticleSystem	: public LkActor
{
	friend class game::LkLevel;

	typedef std::vector<LkParticleSource*>				ParticleSources;
	typedef ParticleSources::iterator					ParticleSourcesIter;
	typedef ParticleSources::const_iterator				ParticleSourcesConstIter;
	typedef ParticleSources::reverse_iterator			ParticleSourcesRIter;
	typedef ParticleSources::const_reverse_iterator		ParticleSourcesConstRIter;
public:

	const LkParticleSystemDescriptor& GetDescriptor() const;
private:
	LkParticleSystem( LkParticleSystemDescriptor& _Descriptor, game::LkLevel* _Level );
	LkParticleSystem();
	~LkParticleSystem();

	void _OnEvent( const LkEvent& _Event );

	LkParticleSystemDescriptor m_Descriptor;
	ParticleSources m_Sources;
	static volatile unsigned int m_SystemCounter;	// Volatile because we might just access it on multiple threads when particle systems
													// are constructed. Don't want multiple particle systems try to construct themselves
													// with the same actor name, that wouldn't work out well.
};

}

#endif