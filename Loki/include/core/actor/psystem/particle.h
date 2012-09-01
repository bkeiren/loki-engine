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
	typedef void( *ParticleCallback )( LkParticle* );

	float m_Age;
	float m_Lifetime;
	glm::vec4 m_Color;	// Color adjuster (Multiplicative), including alpha.
	float m_Size;
	
	void* m_UserData[8];	// Custom userdata, can be used in whatever way is needed by the client.

	bool IsAlive() const;

	//////////////////////////////////////////////////////////////////////////
	// Kill the particle. This effectively disables it and makes it available
	// to the pool of particles of the parent source.
	//////////////////////////////////////////////////////////////////////////
	void Kill();

	//////////////////////////////////////////////////////////////////////////
	// Resurrects the particle by resetting it. This removes it from the
	// available pool of particles of the parent source.
	//////////////////////////////////////////////////////////////////////////
	void Resurrect();

	//////////////////////////////////////////////////////////////////////////
	// Resets the particles orientation and positions and other attributes.
	// Does NOT change whether the particle is alive or dead.
	//////////////////////////////////////////////////////////////////////////
	void Reset();

	//////////////////////////////////////////////////////////////////////////
	// Sets the callback that is used on each frame. If _Callback is 0,
	// a dummy callback will be called internally which does NOT alter
	// the state of the particle in any way.
	//////////////////////////////////////////////////////////////////////////
	void SetCallback( ParticleCallback _Callback );
private:
	LkParticle();
	~LkParticle();

	void _Update();

	bool m_IsAlive;

	ParticleCallback m_Callback;
};

}

#endif