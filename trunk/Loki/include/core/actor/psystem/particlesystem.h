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

namespace renderer
{
	class LkEffect;
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

	const vec3& GetPosition() const;
	void SetPosition( const vec3& _Position );

	void Render( const mat4& _ViewMatrix, const mat4& _ProjectionMatrix );
private:
	LkParticleSystem( LkParticleSystemDescriptor& _Descriptor, const char* _Name, game::LkLevel* _Level );
	LkParticleSystem();
	~LkParticleSystem();

	void _OnEvent( const LkEvent& _Event );

	LkParticleSystemDescriptor m_Descriptor;
	ParticleSources m_Sources;

	static renderer::LkEffect* m_CGEffect;
};

}

#endif